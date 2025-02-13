// Fill out your copyright notice in the Description page of Project Settings.


#include "MapContentsValidator.h"

#include "EngineUtils.h"
#include "MapCheckPlayground.h"
#include "Misc/DataValidation.h"

bool UMapContentsValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
                                                            FDataValidationContext& InContext) const
{
	return InObject != nullptr && InObject->IsA<UWorld>();
}

EDataValidationResult UMapContentsValidator::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData,
	UObject* InAsset, FDataValidationContext& Context)
{
	const auto World = Cast<UWorld>(InAsset);

	for(auto ActorIt = TActorIterator<AActor>(World); ActorIt; ++ActorIt)
	{
		if (ActorIt->IsA<AMapCheckPlayground>())
		{
			return EDataValidationResult::Valid;
		}
	}

	Context.AddError(FText::FromString(TEXT("MapCheckPlayground actor is required")));
	return EDataValidationResult::Invalid;
}
