// Fill out your copyright notice in the Description page of Project Settings.

#include "TBS_CameraPawn.h"


// Sets default values
ATBS_CameraPawn::ATBS_CameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBS_CameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBS_CameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATBS_CameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

