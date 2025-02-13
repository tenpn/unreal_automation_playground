// Fill out your copyright notice in the Description page of Project Settings.


#include "MapCheckPlayground.h"

#include "Misc/UObjectToken.h"

// Sets default values
AMapCheckPlayground::AMapCheckPlayground()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMapCheckPlayground::CheckForErrors()
{
    FMessageLog MapCheck("MapCheck");

    if (bIsBadMapCheck)
    {
        MapCheck.Error()
            ->AddToken(FUObjectToken::Create(this))
            ->AddToken(FTextToken::Create(FText::FromString("has a bad thing!")));
    }
}

// Called when the game starts or when spawned
void AMapCheckPlayground::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapCheckPlayground::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

