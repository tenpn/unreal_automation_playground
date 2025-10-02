// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBalanceData.h"

#include "Misc/DataValidation.h"

#if WITH_EDITOR
EDataValidationResult UGameBalanceData::IsDataValid(FDataValidationContext& Context) const
{
	bool bIsInvalid = false;
	
	if (MaxPlayerHealth<=StartPlayerHealth)
	{
		Context.AddError(FText::FromString("Max health should be more than start health"));
		bIsInvalid=true;
	}

	const auto BaseValiditiy = Super::IsDataValid(Context); 
	return bIsInvalid ? EDataValidationResult::Invalid : BaseValiditiy;
}
#endif