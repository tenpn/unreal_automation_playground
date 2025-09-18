// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "NarrativeInteractable.generated.h"

UCLASS()
class AUTOMATIONPLAYG5_5_API ANarrativeInteractable : public AInteractable
{
	GENERATED_BODY()

public:
	ANarrativeInteractable();
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BPNarrativeVisuals;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseNarrativeVisuals;
	
	virtual void BeginPlay() override;
};
