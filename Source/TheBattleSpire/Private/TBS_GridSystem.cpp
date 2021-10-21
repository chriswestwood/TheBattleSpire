// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GridSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TBS_Hex.h"
#include "TBS_Wall.h"

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
	static ConstructorHelpers::FObjectFinder<UBlueprint> WallBPClass(TEXT("Blueprint'/Game/Blueprints/Level/TBS_WallBP.TBS_WallBP'"));
	if (WallBPClass.Object)
	{
		WallBlueprint = WallBPClass.Object->GeneratedClass;
	}
	LastWorldOffset = FIntPoint(0, 0);
	
}

// Called when the game starts or when spawned
void ATBS_GridSystem::BeginPlay()
{
	Super::BeginPlay();
	GenerateRoom(FIntPoint(7, 7), RoomDirection::RNorth,0);
	
}

// Called every frame
void ATBS_GridSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATBS_GridSystem::GenerateRoom(FIntPoint size, TEnumAsByte<RoomDirection> direction, int doorOffset)
{
	FRoomStruct newRoom = FRoomStruct();

	// calculate the next room types based on current type and seed.
	TEnumAsByte<RoomStyle> newNorthStyle = RoomStyle(FMath::Rand() % 4);
	TEnumAsByte<RoomStyle> newEastStyle = RoomStyle(FMath::Rand() % 4);
	TEnumAsByte<RoomStyle> newSouthStyle = RoomStyle(FMath::Rand() % 4);
	TEnumAsByte<RoomStyle> newWestStyle = RoomStyle(FMath::Rand() % 4);

	int newRoomNumber = 0;
	if (Rooms.IsValidIndex(0)) newRoomNumber = Rooms.Last().GetRoomNumber() + 1;

	// Calculate the new room style based on the previous room
	// also calculate the room offset based on direction
	if (Rooms.Num() == 0)
	{
		newRoom.SetStyle(RoomStyle(FMath::Rand() % 4));
		newSouthStyle = RoomStyle(RoomStyle::Grassland);
	}
	else {
		if (direction == RoomDirection::RNorth)
		{
			newRoom.SetStyle(Rooms.Last().GetNorthStyle());
			newSouthStyle = Rooms.Last().GetStyle();
		}
		else if (direction == RoomDirection::RWest)
		{
			newRoom.SetStyle(Rooms.Last().GetEastStyle());
			newWestStyle = Rooms.Last().GetStyle();
		}
		else if (direction == RoomDirection::RSouth)
		{
			newRoom.SetStyle(Rooms.Last().GetSouthStyle());
			newNorthStyle = Rooms.Last().GetStyle();
		}
		else if (direction == RoomDirection::REast)
		{
			newRoom.SetStyle(Rooms.Last().GetWestStyle());
			newEastStyle = Rooms.Last().GetStyle();
		}
	}

	GenerateNewSquareRoom(size, direction, doorOffset, newRoomNumber);
	//GenerateWallsAndDoors(newRoomNumber);
	// setup the new room variables, add room to array.
	newRoom.SetRoom(size, LastWorldOffset, newRoomNumber, newNorthStyle, newEastStyle, newSouthStyle, newWestStyle);
	Rooms.Add(newRoom);

	// destroy the oldest room so there is only a max of 2
	if (Rooms.Num() > 2) { DestroyOldestRoom(Rooms[0].GetRoomNumber()); }
}

void ATBS_GridSystem::GenerateNewSquareRoom(FIntPoint size, 
											TEnumAsByte<RoomDirection> direction, 
											int doorOffset,
											int roomNumber)
{
	FIntPoint roomOffset = FIntPoint(0, 0);
	//calculate the room offset based on direction
	if (Rooms.Num() == 0);
	else if (direction == RoomDirection::RNorth)
	{	
		roomOffset.X = -size.X + doorOffset + 1;
		roomOffset.Y = Rooms.Last().GetRoomSize().Y;
	}
	else if (direction == RoomDirection::RWest)
	{	
		roomOffset.X = Rooms.Last().GetRoomSize().X;
		roomOffset.Y = -size.Y + doorOffset + 1;
	}
	else if (direction == RoomDirection::RSouth)
	{
		roomOffset.X = -size.X + doorOffset + 1;
		roomOffset.Y = -size.Y;
	}
	else if (direction == RoomDirection::REast)
	{
		roomOffset.X = -size.X;
		roomOffset.Y = -size.Y + doorOffset + 1;
	}
	// add the room offset to the world offset, used to calculate the world positions
	LastWorldOffset.X += roomOffset.X;
	LastWorldOffset.Y += roomOffset.Y;
	// for each y
	for (int j = 0; j < size.Y; ++j)
	{
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
			ATBS_Hex* newHex = GetWorld()->SpawnActor<ATBS_Hex>(HexBlueprint, FVector(x, y, 0), FRotator(0, 0, 0));
			// update any variables the hex tile needs
			newHex->SetGridLocation(FIntPoint(i, j));
			newHex->SetGridRoom(roomNumber);
			if (i == 0 || j == 0 || i == size.X -1 || j == size.Y -1)
			{
				newHex->Tags.Add("Edge");
			}
		}		
	}
}

void ATBS_GridSystem::GenerateWallsAndDoors(int roomNumber)
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATBS_Hex::StaticClass(), "Edge", FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if(Cast<ATBS_Hex>(Actor)->GetGridRoom() == roomNumber) ATBS_Wall* newWall = GetWorld()->SpawnActor<ATBS_Wall>(WallBlueprint, Actor->GetRootComponent()->GetComponentLocation(), FRotator(0,0,0));
	}
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
