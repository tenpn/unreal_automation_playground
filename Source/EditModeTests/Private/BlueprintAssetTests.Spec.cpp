#include "Interactable.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"

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

/** it's hard to fetch all components from a BP, including c++ base class components! use this. */
template<typename BaseT>
void ForAllComponentsOfType(const UBlueprint& BP, TFunctionRef<void (const FString& CompName, const BaseT& Comp)> Handler)
{
	if (const auto BPClass = Cast<UBlueprintGeneratedClass>(BP.GeneratedClass))
	{
		const auto CtorScript = BPClass->SimpleConstructionScript;
		const TArray<USCS_Node*>& CtorNodes = CtorScript->GetAllNodes();
		for(const auto CtorNode : CtorNodes)
		{
			const auto CtorNodeClass = CtorNode->ComponentClass;
			if (CtorNodeClass->IsChildOf(USceneComponent::StaticClass()))
			{
				const auto CtorNodeComponentName = CtorNode->GetVariableName();
				const auto CtorSceneComp = Cast<const USceneComponent>(CtorNode->GetActualComponentTemplate(BPClass));
				Handler(CtorNodeComponentName.ToString(), *CtorSceneComp);
			}
		}
	}

	static TArray<UObject*> SubObjects;
	SubObjects.Empty();
	UObject* const DefaultObject = BP.GeneratedClass->GetDefaultObject();;
	DefaultObject->GetDefaultSubobjects(SubObjects);
	for ( const auto SubObject : SubObjects )
	{
		if ( const auto SceneComponent = Cast<const USceneComponent>(SubObject) )
		{
			Handler(SceneComponent->GetName(), *SceneComponent);
		}
	}
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