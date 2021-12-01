// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_Door.h"
#include "Kismet/GameplayStatics.h"
#include "EnumRoom.h"
#include "TBS_LevelGameMode.h"
#include "TBS_GridSystem.h"
#include "TBS_Hex.h"
#include "TBS_Object.h"

ATBS_Door::ATBS_Door()
{
	bDoorActive = true;
}

bool ATBS_Door::Action(ATBS_Object* pawn)
{
	if (!bDoorActive) return false;
	ATBS_GridSystem* grid = Cast<ATBS_LevelGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetGridSystem();
	if (!grid) return false;
	// check that a player character is in range of the door
	if (Tags.Contains("N"))grid->GenerateNextRoom(RoomDirection::RNorth, GetHex()->GetGridLocation().X);
	if (Tags.Contains("W"))grid->GenerateNextRoom(RoomDirection::RWest, GetHex()->GetGridLocation().Y);
	if (Tags.Contains("S"))grid->GenerateNextRoom(RoomDirection::RSouth, GetHex()->GetGridLocation().X);
	if (Tags.Contains("E"))grid->GenerateNextRoom(RoomDirection::REast, GetHex()->GetGridLocation().Y);
	return true;
}

void ATBS_Door::DeactivateDoor()
{
	bDoorActive = false;
}

