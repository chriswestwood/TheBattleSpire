// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/UserDefinedEnum.h"
#include "EnumRoom.generated.h"


UENUM()
enum RoomStyle
{
	Grassland		UMETA(DisplayName = "Grass"),
	Woodland		UMETA(DisplayName = "Wood"),
	Stone           UMETA(DisplayName = "Stone"),
	MetalRoom		UMETA(DisplayName = "Metal")
};

UENUM()
enum RoomType
{
	Outside     UMETA(DisplayName = "Outside"),
	Inside      UMETA(DisplayName = "Inside"),
	Boss	    UMETA(DisplayName = "BossRoom")
};

UENUM()
enum RoomDirection
{
	North     UMETA(DisplayName = "N"),
	East      UMETA(DisplayName = "E"),
	South	  UMETA(DisplayName = "S"),
	West	  UMETA(DisplayName = "W")
};
/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API UEnumRoomStyle : public UUserDefinedEnum
{
	GENERATED_BODY()
	
};
