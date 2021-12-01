// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_LevelGameMode.h"
#include "TBS_PlayerController.h"
#include "TBS_CameraPawn.h"
#include "TBS_GridSystem.h"

ATBS_LevelGameMode::ATBS_LevelGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> CameraPawnBPClass(TEXT("/Game/Blueprints/Player/TBS_CameraPawnBP"));
	if (CameraPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = CameraPawnBPClass.Class;
	}
	//GameStateClass = ATBS_GameState::StaticClass();
	//PlayerStateClass = ATBS_PlayerState::StaticClass();
	//SpectatorClass = ATBS_SpectatorPawn::StaticClass();
	PlayerControllerClass = ATBS_PlayerController::StaticClass();
	//PlayerStateClass = ATBS_PlayerState::StaticClass();
}

void ATBS_LevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	gridSystem = GetWorld()->SpawnActor<ATBS_GridSystem>(ATBS_GridSystem::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0));
}

ATBS_GridSystem* ATBS_LevelGameMode::GetGridSystem()
{
	return gridSystem;
}
