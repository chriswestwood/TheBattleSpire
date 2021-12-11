// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "EnemyListStruct.generated.h"
class ATBS_EnemyPawn;

USTRUCT(BlueprintType)
struct FEnemyPercentage
{
    GENERATED_USTRUCT_BODY()

public:
    FEnemyPercentage() {}
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TSubclassOf<ATBS_EnemyPawn> enemy_Pawn;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        float percentage;
};



/** Structure that defines a list of enemies to spawn */
USTRUCT(BlueprintType)
struct FEnemyListStruct : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	FEnemyListStruct()
	{}

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TArray<FEnemyPercentage> Enemy;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        int level;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        int count;

	
};