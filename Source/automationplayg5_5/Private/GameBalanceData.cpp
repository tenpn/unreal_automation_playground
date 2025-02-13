// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBalanceData.h"

EDataValidationResult UGameBalanceData::IsDataValid(TArray<FText>& ValidationErrors)
{
	bool bIsInvalid = false;
	
	if (MaxPlayerHealth<=StartPlayerHealth)
	{
		ValidationErrors.Add(FText::FromString("Max health should be more than start health"));
		bIsInvalid=true;
	}
	
	return bIsInvalid ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
