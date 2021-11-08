// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "EnumRoom.h"
#include "LevelAssetStruct.generated.h"
class ATBS_Hex;
class ATBS_Wall;
class ATBS_Door;

/** Structure that defines a set of Level assets */
USTRUCT(BlueprintType)
struct FLevelAssetStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FLevelAssetStruct()
        : room1_Type(RoomType::Type1)
        , room2_Type(RoomType::Type2)
        , room3_Type(RoomType::Type3)
    {}

    /** Row Name is based on level (eg. OrcVillage, IceCave, VampireCastle...) */

    // Room types - defines how each room is to be built, either an outside, enclosed, or indoors arena
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TEnumAsByte<RoomType> room1_Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TEnumAsByte<RoomType> room2_Type;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TEnumAsByte<RoomType> room3_Type;

    // Room Assets - These are the BPs that each room should use based on the level
    // Hex
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TSubclassOf<ATBS_Hex> room1_Hex;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Hex> room2_Hex;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Hex> room3_Hex;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Hex> room4_Hex;
    // Wall Full
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room1_WallFull;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room2_WallFull;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room3_WallFull;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room4_WallFull;
    // Wall Half
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room1_WallHalf;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room2_WallHalf;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room3_WallHalf;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Wall> room4_WallHalf;
    // Door
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Door> room1_Door;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Door> room2_Door;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Door> room3_Door;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
        TSubclassOf<ATBS_Door> room4_Door;

};