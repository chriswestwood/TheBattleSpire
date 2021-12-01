// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "EnumRoom.h"
#include "LevelAssetStruct.generated.h"


/** Structure that defines a set of Level assets */
USTRUCT(BlueprintType)
struct FLevelAssetStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FLevelAssetStruct()
    {}

    // room layout struct
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FName> room_Styles;

    // enemies list
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FName> room_EnemyList;

    //boss
   

};