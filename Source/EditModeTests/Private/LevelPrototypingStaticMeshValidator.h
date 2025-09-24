#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "LevelPrototypingStaticMeshValidator.generated.h"

/**
 * 
 */
UCLASS()
class EDITMODETESTS_API ULevelPrototypingStaticMeshValidator : public UEditorValidatorBase
{
	GENERATED_BODY()
public:
	virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
		FDataValidationContext& InContext) const override;
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
		FDataValidationContext& Context) override;
};
