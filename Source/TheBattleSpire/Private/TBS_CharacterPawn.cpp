// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_CharacterPawn.h"
#include "Components/SpotLightComponent.h"

// Sets default values
ATBS_CharacterPawn::ATBS_CharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!spotLight)
	{
		spotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLight"));
		spotLight->SetRelativeLocation(FVector(-140, 0, 600));
		spotLight->SetRelativeRotation(FRotator(-45, 0, 0));
		spotLight->Intensity = 20000.0f;
		spotLight->AttenuationRadius = 10000.0f;
		spotLight->OuterConeAngle = 44.0f;
		spotLight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}

}

// Called when the game starts or when spawned
void ATBS_CharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBS_CharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

