// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumRoom.generated.h"


UENUM(BlueprintType)
enum RoomType
{
	Type1   UMETA(DisplayName = "Open"),
	Type2	UMETA(DisplayName = "Enclosed"),
	Type3	UMETA(DisplayName = "Building"),
	Type4	UMETA(DisplayName = "None")
};

UENUM()
enum RoomDirection
{
	RNone	UMETA(DisplayName = "Blank"),
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
