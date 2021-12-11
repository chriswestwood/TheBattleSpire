// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Card.generated.h"

UENUM(BlueprintType)
enum CardType
{
	TMoveCard   UMETA(DisplayName = "Move"),
	TAttackCard	UMETA(DisplayName = "Attack")
};

UENUM(BlueprintType)
enum SelectionType
{
	SDirection   UMETA(DisplayName = "Direction"),
	SPoint		UMETA(DisplayName = "Point"),
	SPath		UMETA(DisplayName = "Path"),
	SSelf		UMETA(DisplayName = "Self"),
	STarget		UMETA(DisplayName = "Target"),
	SCone		UMETA(DisplayName = "Cone")
};

/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API UCard : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FName name;
	UPROPERTY()
		TEnumAsByte<CardType> type;
	UPROPERTY()
		TEnumAsByte<SelectionType> selection;
	UPROPERTY()
		int range;


	//image
	//card range
	// selectType
	// AOE


};
