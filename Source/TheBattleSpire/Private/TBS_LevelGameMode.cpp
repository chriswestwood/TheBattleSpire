// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_LevelGameMode.h"
#include "TBS_PlayerController.h"
#include "TBS_PlayerPawn.h"

ATBS_LevelGameMode::ATBS_LevelGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Player/TBS_PlayerPawnBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	//GameStateClass = ATBS_GameState::StaticClass();
	//PlayerStateClass = ATBS_PlayerState::StaticClass();
	//SpectatorClass = ATBS_SpectatorPawn::StaticClass();
	PlayerControllerClass = ATBS_PlayerController::StaticClass();
	//PlayerStateClass = ATBS_PlayerState::StaticClass();

}