#include "EditModeTestUtilities.h"
#include "Interactable.h"
#include "AssetRegistry/AssetRegistryModule.h"

/** to help find all blueprints derived from a specific class */
template<typename BaseT>
void AddChildrenBlueprintsToFilter(FARFilter& Filter)
{
	Filter.TagsAndValues.Add( FBlueprintTags::NativeParentClassPath,FObjectPropertyBase::GetExportPath(BaseT::StaticClass()));

	for ( TObjectIterator<const UClass>It; It; ++It)
	{
		const auto Class = *It;
		if ( Class->IsChildOf(BaseT::StaticClass()) && Class->HasAnyClassFlags(CLASS_Native))
		{
			Filter.TagsAndValues.Add( FBlueprintTags::NativeParentClassPath,FObjectPropertyBase::GetExportPath(Class)); 
		}
	}
	Filter.ClassPaths.Add(UBlueprint::StaticClass()->GetClassPathName());
	Filter.bRecursiveClasses = true;
}

BEGIN_DEFINE_SPEC(
	FInteractableActorsAssetTests,
	"_A.AssetTests.Interactables",
	EAutomationTestFlags::ProductFilter | EAutomationTestFlags::EditorContext)
END_DEFINE_SPEC(FInteractableActorsAssetTests)
void FInteractableActorsAssetTests::Define()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetData;
	
	FARFilter AllInteractablesFilter;
	AllInteractablesFilter.PackagePaths.Add( "/Game" );
	AllInteractablesFilter.bRecursivePaths = true;
	AddChildrenBlueprintsToFilter<AInteractable>(AllInteractablesFilter);
	
	AssetRegistryModule.Get().GetAssets(AllInteractablesFilter, AssetData);
	for ( const FAssetData& Asset : AssetData)
	{
		Describe(Asset.PackageName.ToString(),[this,Asset]()
		{
			It("Has relative transform components",[this,Asset]()
			{
				const auto Blueprint = Cast<const UBlueprint>(Asset.GetAsset());
				TestNotNull("expected blueprint classes only", Blueprint);
				ForAllComponentsOfType<USceneComponent>(*Blueprint, [this](const FString& CompName, const auto& Comp)
				{
					TestFalse("\"" + CompName + "\" using absolute location", Comp.IsUsingAbsoluteLocation());
					TestFalse("\"" + CompName + "\" using absolute rotation", Comp.IsUsingAbsoluteRotation());
					TestFalse("\"" + CompName + "\" using absolute scale", Comp.IsUsingAbsoluteScale());
				});
			});
		});			
	}
}