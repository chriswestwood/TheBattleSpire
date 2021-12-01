// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "EnemyListStruct.generated.h"

USTRUCT(BlueprintType)
struct FEnemyPercentage
{
    GENERATED_USTRUCT_BODY()

public:
    FEnemyPercentage() {}
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    //TSubclassOf<ATBS_Hex> room_hex;
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    //    float percentage;
};



/** Structure that defines a list of enemies to spawn */
USTRUCT(BlueprintType)
struct FEnemyListStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEnemyListStruct()
	{}
	
};