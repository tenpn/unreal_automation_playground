// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapCheckPlayground.generated.h"

UCLASS()
class AUTOMATIONPLAYG5_5_API AMapCheckPlayground : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	bool bIsBadMapCheck = false;

	
public:	
	// Sets default values for this actor's properties
	AMapCheckPlayground();


#if WITH_EDITOR
	virtual void CheckForErrors() override;
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
