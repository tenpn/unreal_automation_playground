// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class AUTOMATIONPLAYG5_5_API AInteractable : public AActor
{
	GENERATED_BODY()

public:
	AInteractable();
	virtual void Tick(float DeltaTime) override;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* BPVisuals;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseVisuals;
	
	virtual void BeginPlay() override;
};
