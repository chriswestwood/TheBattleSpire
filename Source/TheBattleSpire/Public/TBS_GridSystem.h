// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnumRoom.h"
#include "LevelAssetStruct.h"
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
	FIntPoint              roomWorldOffset; // used to calculate where to position new room
	FIntPoint              roomSize; // size of the room x,y
	int                    roomNumber;
	// Room levels
	TEnumAsByte<RoomLevel> roomLevel;
	TEnumAsByte<RoomLevel> roomNorthLevel; 
	TEnumAsByte<RoomLevel> roomEastLevel; 
	TEnumAsByte<RoomLevel> roomSouthLevel; 
	TEnumAsByte<RoomLevel> roomWestLevel; 
	// Door ranges
	FIntPoint			   roomNorthDoorRange;
	FIntPoint			   roomEastDoorRange;
	FIntPoint			   roomSouthDoorRange;
	FIntPoint	           roomWestDoorRange;
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
	TArray<ATBS_Hex*> GenerateNewSquareRoom(FIntPoint size,
			TEnumAsByte<RoomDirection> direction,
			int doorOffset);

	UFUNCTION()
	void GenerateWallsAndDoors(FIntPoint size, TEnumAsByte<RoomDirection> entrance);
		
	UFUNCTION()
	FIntPoint GetDoorRange(int max, TEnumAsByte<RoomLevel> level);

	UFUNCTION()
	TEnumAsByte<RoomLevel> GetRandRoomLevel(TEnumAsByte<RoomLevel> currentLevel);

	// VARIABLES
	UPROPERTY(VisibleDefaultsOnly, Category = Room)
	UDataTable* levelDataTable;
	FLevelAssetStruct* levelDataRow;
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
