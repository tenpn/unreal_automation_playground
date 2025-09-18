#include "Interactable.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/SCS_Node.h"
#include "Engine/SimpleConstructionScript.h"

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
	/*FTopLevelAssetPath TopLevelAssetPath = AInteractable::StaticClass()->GetClassPathName();
	AllAInteractablesFilter.ClassPaths.Add(TopLevelAssetPath);
	AllAInteractablesFilter.ClassPaths.Add(AActor::StaticClass()->GetClassPathName());
	*/
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
				TArray<UObject*> SubObjects;
			
				UObject* const AssetObject = Asset.GetAsset();
				const auto Blueprint = Cast<const UBlueprint>(AssetObject);
				TestNotNull("expected blueprint classes only", Blueprint);
				UObject* DefaultObject = Blueprint->GeneratedClass->GetDefaultObject();;
				DefaultObject->GetDefaultSubobjects(SubObjects);

				if (const auto BPClass = Cast<UBlueprintGeneratedClass>(Blueprint->GeneratedClass))
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
							TestFalse("\"" + CtorNodeComponentName.ToString() + "\" Location", CtorSceneComp->IsUsingAbsoluteLocation());
							TestFalse("\"" + CtorNodeComponentName.ToString() + "\" Rotation", CtorSceneComp->IsUsingAbsoluteRotation());
							TestFalse("\"" + CtorNodeComponentName.ToString() + "\" Scale", CtorSceneComp->IsUsingAbsoluteScale());
						}
					}
				}

				AActor* const Temp = NewObject<AActor>(GetTransientPackage(), Blueprint->GeneratedClass);
				TArray<UActorComponent*> InstancedComponents;
				Temp->GetComponents(InstancedComponents, true);

				for ( const auto SubObject : SubObjects )
				{
					if ( const auto SceneComponent = Cast<const USceneComponent>(SubObject) )
					{
						TestFalse("\"" + SceneComponent->GetName() + "\" Location", SceneComponent->IsUsingAbsoluteLocation());
						TestFalse("\"" + SceneComponent->GetName() + "\" Rotation", SceneComponent->IsUsingAbsoluteRotation());
						TestFalse("\"" + SceneComponent->GetName() + "\" Scale", SceneComponent->IsUsingAbsoluteScale());
					}
				}
			});
		});			
	}
}