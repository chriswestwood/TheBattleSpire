// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_UnitPawn.h"

// Sets default values
ATBS_UnitPawn::ATBS_UnitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATBS_UnitPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBS_UnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

