// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableAssetValidator.h"

#include "EditModeTestUtilities.h"
#include "Interactable.h"
#include "Misc/DataValidation.h"

bool UInteractableAssetValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* const InObject,
                                                                  FDataValidationContext& InContext) const
{
	const auto BPObject = Cast<const UBlueprint>(InObject);
	if (BPObject == nullptr)
	{
		return false;
	}

	if (BPObject->GeneratedClass->IsChildOf(AInteractable::StaticClass()) == false)
	{
		return false;
	}
	
	return true;
}

EDataValidationResult UInteractableAssetValidator::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData,
	UObject* const InAsset, FDataValidationContext& Context)
{
	const auto BPObject = Cast<const UBlueprint>(InAsset);
	ForAllComponentsOfType<const USceneComponent>(*BPObject, [&Context](const FString& CompName, const auto& Comp)
	{
		if (Comp.IsUsingAbsoluteLocation())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Expected component \"%s\" to use relative location"), *CompName)));
		}
		if (Comp.IsUsingAbsoluteRotation())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Expected component \"%s\" to use relative rotation"), *CompName)));
		}
		if (Comp.IsUsingAbsoluteScale())
		{
			Context.AddError(FText::FromString(FString::Printf(TEXT("Expected component \"%s\" to use relative scale"), *CompName)));
		}
	});
	return EDataValidationResult::Valid;
}
