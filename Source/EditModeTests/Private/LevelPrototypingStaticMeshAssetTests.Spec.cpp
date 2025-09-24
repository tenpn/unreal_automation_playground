#include "AssetRegistry/AssetRegistryModule.h"

BEGIN_DEFINE_SPEC(
	FLevelPrototypingStaticMeshAssetTestss,
	"_A.AssetTests.LevelPrototyping.StaticMeshes",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)
	END_DEFINE_SPEC(FLevelPrototypingStaticMeshAssetTestss)
void FLevelPrototypingStaticMeshAssetTestss::Define()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AllStaticMeshAssetDatas;
	
	FARFilter AllStaticMeshesFilter;
	AllStaticMeshesFilter.ClassPaths.Add(UStaticMesh::StaticClass()->GetClassPathName());
	AllStaticMeshesFilter.PackagePaths.Add( "/Game/LevelPrototyping/Meshes" );

	AssetRegistryModule.Get().GetAssets(AllStaticMeshesFilter, AllStaticMeshAssetDatas);
	for(const auto& StaticMeshAssetData : AllStaticMeshAssetDatas)
	{
		Describe(StaticMeshAssetData.AssetName.ToString(), [this,StaticMeshAssetData]
		{
			It("Has navigation data",[this,StaticMeshAssetData]
			{
				const auto StaticMesh = Cast<const UStaticMesh>(StaticMeshAssetData.GetAsset());
				TestTrue("bHasNavigationData", StaticMesh->bHasNavigationData);
			});
		});
	}
}