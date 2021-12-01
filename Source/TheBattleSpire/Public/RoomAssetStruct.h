// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "EnumRoom.h"
#include "RoomAssetStruct.generated.h"
class ATBS_Hex;
class ATBS_Wall;
class ATBS_Door;
class ATBS_Chest;
class ATBS_Object;

USTRUCT(BlueprintType)
struct FObjectPercentage
{
    GENERATED_USTRUCT_BODY()

public:
    FObjectPercentage() {}
    FObjectPercentage(TSubclassOf<ATBS_Object> object, float p)
    {
        room_object = object;
        percentage = p;
    }
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TSubclassOf<ATBS_Object> room_object;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    float percentage;
};

USTRUCT(BlueprintType)
struct FHexPercentage
{
    GENERATED_USTRUCT_BODY()

public:
    FHexPercentage() {}
    FHexPercentage(TSubclassOf<ATBS_Hex> hex, float p)
    {
        room_hex = hex;
        percentage = p;
    }
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TSubclassOf<ATBS_Hex> room_hex;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    float percentage;
};


/** Structure that defines a set of Wall assets */
USTRUCT(BlueprintType)
struct FRoomAssetStruct : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()

public:

    FRoomAssetStruct()
    {}
    // room type (outside, enclosed, inside)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TEnumAsByte<RoomType> room_Type;

    // room details (Min,Max)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    FIntPoint roomSize;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    FIntPoint objectCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    FIntPoint enemyCount;

    // Wall Full
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_FullWalls;

    // Wall Half
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_HalfWalls;

    // Corner
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_CornerWalls;

    // Door
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_Doors;

    // Hex
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FHexPercentage> room_Hexs;

    // Hex Floor Decoration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_Decorations;

    // Obstacles
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_Objects;

    // Hex Chest
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
    TArray<FObjectPercentage> room_Chests;

};
