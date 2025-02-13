#include "CQTest.h"
#include "ObjectBuilder.h"
#include "Components/MapTestSpawner.h"
#include "Helpers/CQTestAssetHelper.h"

TEST_CLASS(FCGBallTests, "Project.ActorTests.Ball")
{
	TUniquePtr<FMapTestSpawner> Spawner;
	AActor* Block;
	float StartZ;
protected:
	BEFORE_EACH()
	{
		Spawner = MakeUnique<FMapTestSpawner>("/Game/FirstPerson/Maps", "FirstPersonMap");
		Spawner->AddWaitUntilLoadedCommand(TestRunner);
		TestCommandBuilder
		    .StartWhen([this](){return nullptr != Spawner->FindFirstPlayerPawn(); }, FTimespan::FromSeconds(30))
		    .Do([this]{
		    	const auto Player = Spawner->FindFirstPlayerPawn();
				const auto BlockClass = CQTestAssetHelper::GetBlueprintClass("SM_ChamferCube3_Blueprint");
		    	const FTransform NearPlayer(Player->GetActorLocation()
		    		+ Player->GetActorForwardVector()*20
		    		+ FVector::UpVector*20);
		    	StartZ = NearPlayer.GetLocation().Z;
		    	Block = &TObjectBuilder<AActor>(*Spawner, BlockClass)
		    	    .Spawn(NearPlayer);
			});
	}
	
	TEST_METHOD(Physics_InAir_Falls)
	{
		TestCommandBuilder
		    .Until([this]
		    {
			    const float FallAmount = StartZ - Block->GetActorLocation().Z;
		    	return FallAmount > 10;
		    },
		    FTimespan::FromSeconds(3));
	}
};