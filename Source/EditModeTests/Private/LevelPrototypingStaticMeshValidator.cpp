#include "LevelPrototypingStaticMeshValidator.h"
#include "Misc/DataValidation.h"

bool ULevelPrototypingStaticMeshValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData,
                                                                           UObject* InObject, FDataValidationContext& InContext) const
{
	return InObject->IsA(UStaticMesh::StaticClass())
	    && InAssetData.PackagePath == FName("/Game/LevelPrototyping/Meshes");
}

EDataValidationResult ULevelPrototypingStaticMeshValidator::ValidateLoadedAsset_Implementation(
	const FAssetData& InAssetData, UObject* InAsset, FDataValidationContext& Context)
{
	const auto StaticMesh = Cast<const UStaticMesh>(InAsset);
	if (StaticMesh->bHasNavigationData == false)
	{
		Context.AddError(FText::FromString("Expected navigation data"));
		return EDataValidationResult::Invalid;
	}
	return EDataValidationResult::Valid;
}
