// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumRoom.generated.h"


UENUM()
enum RoomStyle
{
	Grassland   UMETA(DisplayName = "Grass"),
	Woodland    UMETA(DisplayName = "Wood"),
	Stone       UMETA(DisplayName = "Stone"),
	MetalRoom	UMETA(DisplayName = "Metal")
};

UENUM()
enum RoomType
{
	Outside	UMETA(DisplayName = "Outside"),
	Inside  UMETA(DisplayName = "Inside"),
	Boss	UMETA(DisplayName = "BossRoom")
};

UENUM()
enum RoomDirection
{
	RNorth	UMETA(DisplayName = "N"),
	REast   UMETA(DisplayName = "E"),
	RSouth	UMETA(DisplayName = "S"),
	RWest	UMETA(DisplayName = "W")
};

UENUM()
enum TileDirection
{
	TNorthEast	UMETA(DisplayName = "NE"),
	TNorthWest  UMETA(DisplayName = "NW"),
	TWest		UMETA(DisplayName = "W"),
	TSouthWest	UMETA(DisplayName = "SW"),
	TSouthEast  UMETA(DisplayName = "SE"),
	TEast		UMETA(DisplayName = "E")
};
/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API UEnumRoomStyle : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
