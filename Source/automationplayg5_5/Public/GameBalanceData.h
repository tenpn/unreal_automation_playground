// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameBalanceData.generated.h"

/**
 * 
 */
UCLASS()
class AUTOMATIONPLAYG5_5_API UGameBalanceData : public UDataAsset
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,meta=(ClampMin=0))
	int StartPlayerHealth = 50;

	UPROPERTY(EditAnywhere,meta=(ClampMin=0))
	int MaxPlayerHealth = 100;

public:
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
};
