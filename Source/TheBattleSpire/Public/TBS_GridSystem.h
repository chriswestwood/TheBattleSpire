// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumRoom.h"
#include "LevelAssetStruct.h"
#include "RoomAssetStruct.h"
#include "EnemyListStruct.h"
#include "TBS_GridSystem.generated.h"
class UDataTable;
class ATBS_Hex;

// Room Structure - holds information on each room 
// including: Hex array, room offset, Level num ect. 
USTRUCT()
struct THEBATTLESPIRE_API FRoomStruct
{
	GENERATED_BODY()

	// room stats
	FIntPoint roomWorldOffset; // used to calculate where to position new room
	FIntPoint roomSize; // size of the room x,y
	int roomNumber;
	// Room levels
	int roomLevel;
	int roomNorthLevel; 
	int roomEastLevel;
	int roomSouthLevel;
	int roomWestLevel;
	// Door ranges
	FIntPoint roomNorthDoorRange;
	FIntPoint roomEastDoorRange;
	FIntPoint roomSouthDoorRange;
	FIntPoint roomWestDoorRange;
	TEnumAsByte<RoomDirection> entrance;
	TArray<ATBS_Hex*> hexes;
	// set defaults for a room
	FRoomStruct()
	{

	}

};

UCLASS()
class THEBATTLESPIRE_API ATBS_GridSystem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBS_GridSystem();
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// generate the Starting room.
	UFUNCTION(BlueprintCallable, Category = Build)
	void GenerateStartRoom();

	// Main function to generate a Room, calls GenerateNewSquareRoom to build grid
	UFUNCTION(BlueprintCallable, Category = Build)
	void GenerateNextRoom(TEnumAsByte<RoomDirection> direction,
						int doorOffset);
	
	void DestroyOldRooms();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Generate a Square grid of Hex tiles as a room 
	// set in the direction and offset provided
	UFUNCTION()
		TArray<ATBS_Hex*> GenerateNewSquareRoom(struct FRoomStruct& room,
			int doorOffset);

	UFUNCTION()
	void GenerateWallsAndDoors(struct FRoomStruct& room);

	UFUNCTION()
	void GenerateInterior(struct FRoomStruct& room);
		
	UFUNCTION()
	FIntPoint GetDoorRange(int max, TEnumAsByte<RoomType> type);

	UFUNCTION()
	int GetRandRoomLevel(int currentRoomLevel);

	UFUNCTION()
	TSubclassOf<ATBS_Hex> GetRandHex(TArray<FHexPercentage> Hexes);
	UFUNCTION()
	TSubclassOf<ATBS_Object> GetRandObject(TArray<FObjectPercentage> Objects, bool returnLastOnNone = false);

	// VARIABLES
	UPROPERTY(VisibleDefaultsOnly, Category = Room)
	UDataTable* levelDataTable;
	UDataTable* roomDataTable;
	FLevelAssetStruct* levelDataRow;
	TArray<FRoomAssetStruct*> roomDataRows;
	TArray<FEnemyListStruct*> EnemyList;
	UPROPERTY()
	TArray<FRoomStruct> Rooms;
	UPROPERTY(VisibleAnywhere, Category = Room)
	FIntPoint LastWorldOffset;
	UPROPERTY(VisibleAnywhere, Category= Room)
	int currentRoomCount;
	UPROPERTY(VisibleAnywhere, Category = Room)
	int maxRoomsShownCount;

	UPROPERTY()
	TSubclassOf<ATBS_Hex> spawnHex;

};
