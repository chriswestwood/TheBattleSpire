// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_CharacterPawn.h"

// Sets default values
ATBS_CharacterPawn::ATBS_CharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

