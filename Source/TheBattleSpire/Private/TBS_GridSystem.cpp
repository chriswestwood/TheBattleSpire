// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GridSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
ATBS_GridSystem::ATBS_GridSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UBlueprint> HexBPClass(TEXT("Blueprint'/Game/Blueprints/Level/TBS_HexBP.TBS_HexBP'"));
	if (HexBPClass.Object)
	{
		HexBlueprint = HexBPClass.Object->GeneratedClass;
	}
	LastWorldOffset = FIntPoint(0, 0);
	
}

// Called when the game starts or when spawned
void ATBS_GridSystem::BeginPlay()
{
	Super::BeginPlay();
	GenerateNewSquareRoom(FIntPoint(7, 7), RoomDirection::North, 0);
	
}

// Called every frame
void ATBS_GridSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATBS_GridSystem::GenerateNewSquareRoom(FIntPoint size, TEnumAsByte<RoomDirection> direction, int doorOffset)
{
	FRoomStruct newRoom = FRoomStruct();
	int newRoomNumber = 0;
	if (Rooms.IsValidIndex(0)) newRoomNumber = Rooms.Last().GetRoomNumber() + 1;
	FIntPoint roomOffset = FIntPoint(0,0);

	// calculate the next room types based on current type and seed.
	TEnumAsByte<RoomStyle> newNorthStyle = RoomStyle(FMath::Rand() % 4);
	TEnumAsByte<RoomStyle> newEastStyle = RoomStyle(FMath::Rand() % 4);
	TEnumAsByte<RoomStyle> newSouthStyle = RoomStyle(FMath::Rand() % 4);
	TEnumAsByte<RoomStyle> newWestStyle = RoomStyle(FMath::Rand() % 4);

	// Calculate the new room style based on the previous room
	// also calculate the room offset based on direction
	if (Rooms.Num() == 0)
	{
		newRoom.SetStyle(RoomStyle(FMath::Rand() % 4));
		newSouthStyle = RoomStyle(RoomStyle::Grassland);
	}
	else {
		if (direction == RoomDirection::North)
		{
			newRoom.SetStyle(Rooms.Last().GetNorthStyle());
			newSouthStyle = Rooms.Last().GetStyle();
			roomOffset.X = -size.X + doorOffset + 1;
			roomOffset.Y = Rooms.Last().GetRoomSize().Y;
		}
		else if (direction == RoomDirection::West)
		{
			newRoom.SetStyle(Rooms.Last().GetEastStyle());
			newWestStyle = Rooms.Last().GetStyle();
			roomOffset.X = Rooms.Last().GetRoomSize().X;
			roomOffset.Y = -size.Y + doorOffset + 1;
		}
		else if (direction == RoomDirection::South)
		{
			newRoom.SetStyle(Rooms.Last().GetSouthStyle());
			newNorthStyle = Rooms.Last().GetStyle();
			roomOffset.X =  -size.X + doorOffset + 1;
			roomOffset.Y = -size.Y;
		}
		else if (direction == RoomDirection::East)
		{
			newRoom.SetStyle(Rooms.Last().GetWestStyle());
			newEastStyle = Rooms.Last().GetStyle();
			roomOffset.X = -size.X;
			roomOffset.Y = -size.Y + doorOffset + 1;
		}
	}

	// Default the rotation to 0,0,0
	FRotator rotation = FRotator(0,0,0);
	// add the room offset to the world offset, used to calculate the world positions
	LastWorldOffset.X += roomOffset.X;
	LastWorldOffset.Y += roomOffset.Y;
	// generate the grid of hex tiles and set locations
	TArray<TArray<ATBS_Hex*>> newArray; // new grid array
	// for each y
	for (int j = 0; j < size.Y; ++j)
	{
		TArray<ATBS_Hex*> xArray; // new x array
		// for each x
		for (int i = 0; i < size.X; ++i)
		{
			// calculate the x and y positions of the new hex tile 
			// if y is even add an offset to x
			int x = ((i + LastWorldOffset.X) * 260);
			if ((j + LastWorldOffset.Y) % 2 == 0)
			{
				x += 130;
			}
			int y = (j + LastWorldOffset.Y) * 225;
			// create the new hex tile
			ATBS_Hex* newHex = GetWorld()->SpawnActor<ATBS_Hex>(HexBlueprint, FVector(x, y, 0), rotation);
			// update any variables the hex tile needs
			newHex->SetGridLocation(FIntPoint(i, j));
			newHex->SetGridRoom(newRoomNumber);
			// add tile to the x Array
			xArray.Add(newHex);
		}		
		// add the x Array to the 2D array 
		newArray.Add(xArray);
	}
	// setup the new room variables, add room to array.
	newRoom.SetRoom(size, roomOffset, LastWorldOffset, newRoomNumber, newNorthStyle, newEastStyle, newSouthStyle, newWestStyle, newArray);
	Rooms.Add(newRoom);

	// destroy the oldest room so there is only a max of 2
	if (Rooms.Num() > 2) { DestroyOldestRoom(Rooms[0].GetRoomNumber()); }
}

void ATBS_GridSystem::DestroyOldestRoom(int roomNum)
{
	// find all actors of hex
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBS_Hex::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (Cast<ATBS_Hex>(Actor)->GetGridRoom() == roomNum)
		{
			// Detroy hex if matching room number
			Cast<ATBS_Hex>(Actor)->Drop();
		}
	}
	Rooms.RemoveAt(0);
}

TArray<TArray<ATBS_Hex*>> FRoomStruct::GetHexArray()
{
	return hexArray;
}
