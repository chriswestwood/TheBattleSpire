// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TBS_Hex.h"
#include "EnumRoom.h"
#include "TBS_GridSystem.generated.h"

// Room Structure - holds information on each room 
// including: Hex array, room offset, type ect. 
USTRUCT()
struct THEBATTLESPIRE_API FRoomStruct
{
	GENERATED_BODY()
protected:
	FIntPoint roomOffset; // offset of last room, to calculate if we are returning
	FIntPoint worldOffset; // used to calculate where to position new room
	FIntPoint roomSize; // size of the room x,y
	int roomNumber;
	TEnumAsByte<RoomStyle> roomStyle; // Current room style
	TEnumAsByte<RoomStyle> roomNorthStyle; // North room style
	TEnumAsByte<RoomStyle> roomEastStyle; // East room style
	TEnumAsByte<RoomStyle> roomSouthStyle; // South room style
	TEnumAsByte<RoomStyle> roomWestStyle; // West room style
	TEnumAsByte<RoomType> roomType; // Current room type
	TArray<TArray<ATBS_Hex*>> hexArray; // array of Hex tiles that make up room

public:
	FIntPoint GetOffset() { return roomOffset; }
	FVector2D GetWorldOffset() { return worldOffset; }
	FIntPoint GetRoomSize() { return roomSize; }
	int GetRoomNumber() { return roomNumber; }
	ATBS_Hex* GetHex(int x, int y) { return hexArray[x][y];}
	TArray<TArray<ATBS_Hex*>> GetHexArray();
	TEnumAsByte<RoomStyle> GetStyle() { return roomStyle; }
	TEnumAsByte<RoomStyle> GetNorthStyle() { return roomNorthStyle; }
	TEnumAsByte<RoomStyle> GetEastStyle() { return roomEastStyle; }
	TEnumAsByte<RoomStyle> GetSouthStyle() { return roomSouthStyle; }
	TEnumAsByte<RoomStyle> GetWestStyle() { return roomWestStyle; }

	void SetType(TEnumAsByte<RoomType> newType) { roomType = newType; }
	void SetStyle(TEnumAsByte<RoomStyle> newStyle, bool bIsBoss = false)
	{
		roomStyle = newStyle;
		if (bIsBoss) { SetType(RoomType::Boss); return;}
		if (roomStyle = RoomStyle::Grassland)
		{
			SetType(RoomType::Outside);
		}
		else
		{
			SetType(RoomType::Inside);
		}
	}

	void SetRoom(FIntPoint newSize,
					FIntPoint newRoomOff,
					FIntPoint newWorldOff,
					int newRoomNumber,
					TEnumAsByte<RoomStyle> newNorthStyle,
					TEnumAsByte<RoomStyle> newEastStyle,
					TEnumAsByte<RoomStyle> newSouthStyle,
					TEnumAsByte<RoomStyle> newWestStyle,
					TArray<TArray<ATBS_Hex*>> newArray)
	{
		roomSize = newSize;
		roomOffset = newRoomOff;
		worldOffset = newWorldOff;
		roomNumber = newRoomNumber;
		roomNorthStyle = newNorthStyle;
		roomEastStyle = newEastStyle;
		roomSouthStyle = newSouthStyle;
		roomWestStyle = newWestStyle;
		hexArray = newArray;
	}

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

	// Generate a Square grid of Hex tiles as a room 
	// set in the direction and offset provided
	UFUNCTION(blueprintcallable, Category = build)
	void GenerateNewSquareRoom(FIntPoint size,
						TEnumAsByte<RoomDirection> direction,
						int doorOffset);

	void DestroyOldestRoom(int roomNum);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// VARIABLES
	TSubclassOf<ATBS_Hex> HexBlueprint;
	TArray<FRoomStruct> Rooms;

	FIntPoint LastWorldOffset;
	int roomCount;

};