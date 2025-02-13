#include "FileHelpers.h"
#include "AssetRegistry/AssetRegistryModule.h"
BEGIN_DEFINE_SPEC(
	FLevelAssetTests,
	"Project.AssetTests.Levels",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)
END_DEFINE_SPEC(FLevelAssetTests)
void FLevelAssetTests::Define()
{
    // find all levels
    FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
    TArray<FAssetData> AssetData;
    
    FARFilter AllLevelsFilter;
    AllLevelsFilter.ClassPaths.Add(UWorld::StaticClass()->GetClassPathName());
    AssetRegistryModule.Get().GetAssets(AllLevelsFilter, AssetData);
    
    for (const FAssetData& Asset : AssetData)
    {
    	Describe(FString::Printf(TEXT("Level: %ls"), *Asset.AssetName.ToString()), [this,Asset]()
	    {
		    BeforeEach([this,Asset]()
		    {
		    	const auto CurrentMap = GEditor->GetEditorWorldContext().World()->GetPackage()->GetPathName();
		    	if (CurrentMap != Asset.PackageName.ToString())
		    	{
		    		UE_LOG(LogTemp, Log, TEXT("loading level %s from %s"), *Asset.PackageName.ToString(), *CurrentMap);
		    		UEditorLoadingAndSavingUtils::LoadMap(Asset.PackageName.ToString());
		    	}
		    });
    		
		    It("All barrels on correct layer", [this]()
		    {
			    UE_LOG(LogTemp, Log, TEXT("barrels test"));
		    });

		    It("At least one spawn", [this]()
		    {
			    UE_LOG(LogTemp, Log, TEXT("spawn test"));
		    });

    		AfterEach([this]()
    		{
    		});
	    });
    }
}