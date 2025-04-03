// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapGenerator.generated.h"

UCLASS()
class AUTOMATIONPLAYG5_5_API AMapGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapGenerator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool TrySetSeed(uint32 NewSeed)
	{
		bool bIsNew = m_Seed != NewSeed; 
		m_Seed = NewSeed;
		return bIsNew;
	}

	void Generate()
	{
		UE_LOG(LogTemp, Log, TEXT("GENERATING with seed %ud"), m_Seed); 
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	uint32 m_Seed=0;

};
