// Fill out your copyright notice in the Description page of Project Settings.


#include "NarrativeInteractable.h"


// Sets default values
ANarrativeInteractable::ANarrativeInteractable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseNarrativeVisuals = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Narrative Actor Visuals"));
	BaseNarrativeVisuals->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ANarrativeInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANarrativeInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

