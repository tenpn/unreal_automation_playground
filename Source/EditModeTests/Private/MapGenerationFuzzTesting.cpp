#include "EngineUtils.h"
#include "FileHelpers.h"
#include "LevelEditor.h"
#include "MapGenerator.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Tests/AutomationEditorCommon.h"

namespace
{
const FString TargetMap = "/Game/FirstPerson/Maps/FirstPersonMap";

DEFINE_LATENT_AUTOMATION_COMMAND(FWaitForPIEBoot);
bool FWaitForPIEBoot::Update()
{
	if (GEditor->IsPlayingSessionInEditor() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("not playing yet"));
		return false;
	}

	const auto PlayWorld = GEditor->PlayWorld.Get(); 
	if (PlayWorld == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("no world yet"));
		return false;
	}

	if (PlayWorld->AreActorsInitialized() == false)
	{
		UE_LOG(LogTemp, Log, TEXT("actors not booted"));
		return false;
	}
	UE_LOG(LogTemp, Log, TEXT("...done"));
	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FCallDoneDelegate, FDoneDelegate, Done);
bool FCallDoneDelegate::Update()
{
	Done.Execute();
	return true;
}
	
bool StartPIE(bool bSimulateInEditor)
{
	FLevelEditorModule& LevelEditorModule = FModuleManager::Get().GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));

	FRequestPlaySessionParams Params;
	Params.DestinationSlateViewport = LevelEditorModule.GetFirstActiveViewport();
	if (bSimulateInEditor)
	{
		Params.WorldType = EPlaySessionWorldType::SimulateInEditor;
	}

	// Make sure the player start location is a valid location.
	if (GUnrealEd->CheckForPlayerStart() == nullptr)
	{
		FAutomationEditorCommonUtils::SetPlaySessionStartToActiveViewport(Params);
	}

	GUnrealEd->RequestPlaySession(Params);

	return true;
}	
}

BEGIN_DEFINE_SPEC(
	FMapGenerationFuzzTesting,
	"_A.MapGeneration.FuzzTesting",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)

TWeakObjectPtr<AMapGenerator> MapGenerator = nullptr;

void CreateGenerationTests(uint32 Seed)
{
	BeforeEach([this,Seed]()
	{
		static const auto CurrentMap = GEditor->GetEditorWorldContext().World()->GetPackage()->GetPathName();
			
		if (CurrentMap != TargetMap)
		{
			UE_LOG(LogTemp, Log, TEXT("loaded map instead of %s"), *CurrentMap);
			UEditorLoadingAndSavingUtils::LoadMap(TargetMap);
		}
		if (MapGenerator.Get() == nullptr)
		{
			for (TActorIterator<AMapGenerator> ActorItr(GEditor->GetEditorWorldContext().World()); ActorItr; ++ActorItr)
			{
				if (auto* const MaybeGenerator = Cast<AMapGenerator>(*ActorItr))
				{
					UE_LOG(LogTemp, Log, TEXT("found generator"));
					MapGenerator = MaybeGenerator;
					break;
				}
			}
		}
		TestTrue("Expected to find map generator in level", MapGenerator.IsValid());

		if (MapGenerator->TrySetSeed(Seed))
		{
			AddInfo(FString::Printf(TEXT("set new seed %ud"), Seed));
			MapGenerator->Generate();
		}
	});
	
	It("(Fails on seed 99) Meets Another Heuristic", [this,Seed]()
	{
		UE_LOG(LogTemp, Log, TEXT("first test"));
		float MapHeuristic = Seed == 99 ? 1 : 0; // MapGenerator->EvaluateSomeHeuristic()
		TestLessThan("heuristic", MapHeuristic, 0.5f);
	});

	It("(Always passes) Meets Some Heuristic", [this]()
	{
		UE_LOG(LogTemp, Log, TEXT("second test"));
		float MapHeuristic = 0; // MapGenerator->EvaluateAnotherHeuristic()
		TestLessThan("heuristic", MapHeuristic, 0.6f);
	});

	Describe("Validated in PIE", [this]
	{
		BeforeEach([this]
		{
			StartPIE(true);
		});
		BeforeEach(EAsyncExecution::ThreadPool, []
		{
			UE_LOG(LogTemp, Log, TEXT("awaiting session"));
			while(GEditor->IsPlayingSessionInEditor() == false)
			{
				FGenericPlatformProcess::Yield();
			}
			UE_LOG(LogTemp, Log, TEXT("awaiting world"));
			while (GEditor->PlayWorld.Get() == nullptr)
			{
				FGenericPlatformProcess::Yield();
			}
			const auto PlayWorld = GEditor->PlayWorld.Get();
			UE_LOG(LogTemp, Log, TEXT("awaiting actors"));
			while (PlayWorld->AreActorsInitialized() == false)
			{
				FGenericPlatformProcess::Yield();
			}
		});

		It("Has runtime values", [this]
		{
			TestTrue("expected to be playing", GEditor->IsPlayingSessionInEditor());
			TestNotNull("expected world", GEditor->PlayWorld.Get());
			TestTrue("expected init actors", GEditor->PlayWorld->AreActorsInitialized());
			AddInfo("Woo it's runtime!!");
		});

		It("Another runtime test without exiting PIE", [this]
		{
			TestTrue("expected to be playing", GEditor->IsPlayingSessionInEditor());
			TestNotNull("expected world", GEditor->PlayWorld.Get());
			TestTrue("expected init actors", GEditor->PlayWorld->AreActorsInitialized());
			AddInfo("Woo it's STILL runtime!!");
		});

		// Caution: Order of these statements is important

		AfterEach(EAsyncExecution::ThreadPool, []
		{
			while (GEditor->IsPlayingSessionInEditor())
			{
				// Wait until play session ends
				FGenericPlatformProcess::Yield();
			}
		});

		AfterEach([]
		{
			// Request end of play session
			GUnrealEd->RequestEndPlayMap();
		});
	});
}

END_DEFINE_SPEC(FMapGenerationFuzzTesting)

void FMapGenerationFuzzTesting::Define()
{
	Describe("Generate map with random seeds", [this]
	{
		constexpr auto RandomSeedCount = 10;
		static TArray<uint32> UsedSeeds;
		UsedSeeds.Empty(RandomSeedCount);
		for (int32 Index = 0; Index < RandomSeedCount; ++Index)
		{
			// can't repeat seeds
			uint32 RandomSeed = 0;
			do
			{
				RandomSeed = FMath::RandRange(0, 99);
			} while(UsedSeeds.Contains(RandomSeed));
			UsedSeeds.Add(RandomSeed);
			
			Describe(FString::Printf(TEXT("Random seed %ud"), RandomSeed), [this,RandomSeed]
			{
				CreateGenerationTests(RandomSeed);
			});
		}
	});
	
	Describe("Generate map with fixed seeds", [this]
	{
		static TArray<uint32> FixedSeeds = { 1, 2, 3, 99, 50 };
		for (const auto FixedSeed : FixedSeeds)
		{
			Describe(FString::Printf(TEXT("Fixed seed %ud"), FixedSeed), [this,FixedSeed]
			{
				CreateGenerationTests(FixedSeed);
			});
		}
	});
		
}
