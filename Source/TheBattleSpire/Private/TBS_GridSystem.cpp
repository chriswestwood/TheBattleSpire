// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GridSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TBS_Hex.h"
#include "TBS_Wall.h"
#include "TBS_Door.h"
#include "TBS_GameInstance.h"

// Sets default values
ATBS_GridSystem::ATBS_GridSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableClass(TEXT("DataTable'/Game/Data/LevelAssetData.LevelAssetData'"));
	if (DataTableClass.Object)
	{
		levelDataTable = DataTableClass.Object;
	}
	static ConstructorHelpers::FClassFinder<ATBS_Hex> spawnHexClass(TEXT("/Game/Blueprints/Level/Hexes/TBS_Hex_Spawn"));
	if (spawnHexClass.Class)
	{
		spawnHex = spawnHexClass.Class;
	}

	LastWorldOffset = FIntPoint(0, 0);
	currentRoomCount = 0;
	maxRoomsShownCount = 1;
}

// Called when the game starts or when spawned
void ATBS_GridSystem::BeginPlay()
{
	Super::BeginPlay();
	UTBS_GameInstance* GI = Cast<UTBS_GameInstance>(GetGameInstance());
	if(GI && levelDataTable) levelDataRow = levelDataTable->FindRow<FLevelAssetStruct>("OrcVillage", "");
	if(levelDataRow) GenerateStartRoom();
}

// Called every frame
void ATBS_GridSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATBS_GridSystem::GenerateStartRoom()
{
	FRoomStruct startRoom = FRoomStruct();
	startRoom.roomLevel = Level0;
	// setup the new room variables, add room to array.
	startRoom.roomSize = FIntPoint(7, 7);
	startRoom.roomWorldOffset = FIntPoint(0, 0);
	startRoom.roomNumber = 0;
	startRoom.roomLevel = Level0;
	startRoom.roomNorthLevel = Level1;
	startRoom.roomEastLevel = Level1;
	startRoom.roomSouthLevel = Level1;
	startRoom.roomWestLevel = Level1;
	startRoom.roomNorthDoorRange = GetDoorRange(startRoom.roomSize.X, startRoom.roomNorthLevel);
	startRoom.roomEastDoorRange = GetDoorRange(startRoom.roomSize.Y, startRoom.roomEastLevel);
	startRoom.roomSouthDoorRange = GetDoorRange(startRoom.roomSize.X, startRoom.roomSouthLevel);
	startRoom.roomWestDoorRange = GetDoorRange(startRoom.roomSize.Y, startRoom.roomWestLevel);
	Rooms.Add(startRoom);

	// create new room
	GenerateNewSquareRoom(startRoom.roomSize, RNone, 0);
	GenerateWallsAndDoors(startRoom.roomSize, RNone);
	currentRoomCount += 1;
}

void ATBS_GridSystem::GenerateNextRoom(TEnumAsByte<RoomDirection> direction, int doorOffset)
{
	if (direction == RNone) return;
	// Deactivate doors currently in play
	// any Door from entrace is removed instead
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBS_Door::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ATBS_Door* door = Cast<ATBS_Door>(Actor);
		door->DeactivateDoor();
		if (direction == RNorth && door->Tags.Contains("N")) door->Destroy();
		if (direction == REast && door->Tags.Contains("E")) door->Destroy();
		if (direction == RSouth && door->Tags.Contains("S")) door->Destroy();
		if (direction == RWest && door->Tags.Contains("W")) door->Destroy();
	}

	// create new room
	FRoomStruct newRoom = FRoomStruct();

	// calculate the next room level
	// Also Generate next rooms levels - Based on current Level and amount of rooms already entered
	// Level in opposite direction is same as current
	if (direction == RNorth)
	{
		newRoom.roomLevel = Rooms.Last().roomNorthLevel;
		newRoom.roomNorthLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomEastLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomSouthLevel = newRoom.roomLevel;
		newRoom.roomWestLevel = GetRandRoomLevel(newRoom.roomLevel);
	}
	else if (direction == RWest)
	{
		newRoom.roomLevel = Rooms.Last().roomEastLevel;
		newRoom.roomNorthLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomEastLevel = newRoom.roomLevel;
		newRoom.roomSouthLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomWestLevel = GetRandRoomLevel(newRoom.roomLevel);
	}
	else if (direction == RSouth)
	{
		newRoom.roomLevel = Rooms.Last().roomSouthLevel;
		newRoom.roomNorthLevel = newRoom.roomLevel;
		newRoom.roomEastLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomSouthLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomWestLevel = GetRandRoomLevel(newRoom.roomLevel);
	}
	else if (direction == REast)
	{
		newRoom.roomLevel =  Rooms.Last().roomEastLevel;
		newRoom.roomNorthLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomEastLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomSouthLevel = GetRandRoomLevel(newRoom.roomLevel);
		newRoom.roomWestLevel = newRoom.roomLevel;
	}

	// get room size - TODO
	newRoom.roomSize = FIntPoint(FMath::FRandRange(9, 15), FMath::FRandRange(9, 15));


	// calc Door ranges
	newRoom.roomNorthDoorRange =GetDoorRange(newRoom.roomSize.X, newRoom.roomNorthLevel);
	newRoom.roomEastDoorRange = GetDoorRange(newRoom.roomSize.Y, newRoom.roomEastLevel);
	newRoom.roomSouthDoorRange = GetDoorRange(newRoom.roomSize.X, newRoom.roomSouthLevel);
	newRoom.roomWestDoorRange = GetDoorRange(newRoom.roomSize.Y, newRoom.roomWestLevel);

	// setup the new room variables, add room to array.
	newRoom.roomWorldOffset = LastWorldOffset;
	newRoom.roomNumber = currentRoomCount;
	Rooms.Add(newRoom);

	// destroy the oldest room so there is only a max of 2
	DestroyOldRooms();
	// create new room
	GenerateNewSquareRoom(newRoom.roomSize, direction, doorOffset);
	GenerateWallsAndDoors(newRoom.roomSize, direction);
	currentRoomCount += 1;
}

TArray<ATBS_Hex*> ATBS_GridSystem::GenerateNewSquareRoom(FIntPoint size,
											TEnumAsByte<RoomDirection> direction, 
											int doorOffset)
{
	FIntPoint roomOffset;
	TArray<ATBS_Hex*> Hexes;
	// calculate the room offset based on several factors
	// room needs to be connected to all doors from previous room
	// direction plays into which offset will be randomised
	//
	if (direction == RNorth)
	{
		roomOffset.X = -size.X + doorOffset + 4;
		roomOffset.Y = Rooms[Rooms.Num()-2].roomSize.Y;
	}
	else if (direction == RWest)
	{
		roomOffset.X = Rooms[Rooms.Num() - 2].roomSize.X;
		roomOffset.Y = -size.Y + doorOffset + 4;
	}
	else if (direction == RSouth)
	{
		roomOffset.X = -size.X + doorOffset + 4;
		roomOffset.Y = -size.Y;
	}
	else if (direction == REast)
	{
		roomOffset.X = -size.X;
		roomOffset.Y = -size.Y + doorOffset + 4;
	}
	else roomOffset = FIntPoint(0, 0);

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
			ATBS_Hex* newHex;

			// Spawn Hex based on where it is located in the grid
			if (i == 0)
			{
				if (Rooms.Last().roomEastLevel == Level1)      newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room1_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomEastLevel == Level2) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room2_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomEastLevel == Level3) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room3_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomEastLevel == Level4) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomEastLevel == Level5) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else                                            newHex = GetWorld()->SpawnActor<ATBS_Hex>(spawnHex, FVector(x, y, 0), FRotator(0, 0, 0));
				newHex->Tags.Add("E");
				newHex->Tags.AddUnique("Edge");
				newHex->SetRoomLevel(Rooms.Last().roomEastLevel);
			}
			else if (i == size.X - 1)
			{
				if (Rooms.Last().roomWestLevel == Level1)      newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room1_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomWestLevel == Level2) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room2_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomWestLevel == Level3) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room3_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomWestLevel == Level4) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomWestLevel == Level5) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else                                            newHex = GetWorld()->SpawnActor<ATBS_Hex>(spawnHex, FVector(x, y, 0), FRotator(0, 0, 0));
				newHex->Tags.Add("W");
				newHex->Tags.AddUnique("Edge");
				newHex->SetRoomLevel(Rooms.Last().roomWestLevel);
			}
			else if (j == 0)
			{
				if (Rooms.Last().roomSouthLevel == Level1)      newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room1_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomSouthLevel == Level2) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room2_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomSouthLevel == Level3) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room3_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomSouthLevel == Level4) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomSouthLevel == Level5) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else                                             newHex = GetWorld()->SpawnActor<ATBS_Hex>(spawnHex, FVector(x, y, 0), FRotator(0, 0, 0));
				newHex->Tags.Add("S");
				newHex->Tags.AddUnique("Edge");
				newHex->SetRoomLevel(Rooms.Last().roomSouthLevel);
			}
			else if (j == size.Y - 1)
			{
				if (Rooms.Last().roomNorthLevel == Level1)      newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room1_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomNorthLevel == Level2) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room2_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomNorthLevel == Level3) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room3_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomNorthLevel == Level4) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomNorthLevel == Level5) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else                                             newHex = GetWorld()->SpawnActor<ATBS_Hex>(spawnHex, FVector(x, y, 0), FRotator(0, 0, 0));
				newHex->Tags.Add("N");
				newHex->Tags.AddUnique("Edge");
				newHex->SetRoomLevel(Rooms.Last().roomNorthLevel);
			}
			else
			{
				if (Rooms.Last().roomLevel == Level1)      newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room1_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomLevel == Level2) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room2_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomLevel == Level3) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room3_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomLevel == Level4) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else if (Rooms.Last().roomLevel == Level5) newHex = GetWorld()->SpawnActor<ATBS_Hex>(levelDataRow->room4_Hex.Get(), FVector(x, y, 0), FRotator(0, 0, 0));
				else                                            newHex = GetWorld()->SpawnActor<ATBS_Hex>(spawnHex, FVector(x, y, 0), FRotator(0, 0, 0));
				newHex->SetRoomLevel(Rooms.Last().roomLevel);
			}

			newHex->SetGridLocation(FIntPoint(i, j));
			newHex->SetGridRoom(currentRoomCount);
			Hexes.Add(newHex);
		}		
	}
	return Hexes;
}

void ATBS_GridSystem::GenerateWallsAndDoors(FIntPoint size, TEnumAsByte<RoomDirection> entrance)
{
	TArray<AActor*> FoundActors;

	FIntPoint NorthDoorRange = Rooms.Last().roomNorthDoorRange;  
	FIntPoint EastDoorRange = Rooms.Last().roomEastDoorRange; 
	FIntPoint SouthDoorRange = Rooms.Last().roomSouthDoorRange;
	FIntPoint WestDoorRange = Rooms.Last().roomWestDoorRange; 

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATBS_Hex::StaticClass(), "Edge", FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ATBS_Hex* hex = Cast<ATBS_Hex>(Actor);
		if (hex && hex->GetGridRoom() == currentRoomCount)
		{
			bool bHasHexNW = hex->HasNeighbourHex(TNorthWest);
			bool bHasHexNE = hex->HasNeighbourHex(TNorthEast);
			bool bHasHexE = hex->HasNeighbourHex(TEast);
			bool bHasHexSE = hex->HasNeighbourHex(TSouthEast);
			bool bHasHexSW = hex->HasNeighbourHex(TSouthWest);
			bool bHasHexW = hex->HasNeighbourHex(TWest);
			// check for door, ensure the entrance wall does not have a door added
			// door is added if hex pos is within the range found above
			if ((entrance != RSouth && hex->Tags.Contains("N") && NorthDoorRange.X <= hex->GetGridLocation().X && hex->GetGridLocation().X <= NorthDoorRange.Y) ||
				(entrance != RWest && hex->Tags.Contains("E") && EastDoorRange.X <= hex->GetGridLocation().Y && hex->GetGridLocation().Y <= EastDoorRange.Y) ||
				(entrance != RNorth && hex->Tags.Contains("S") && SouthDoorRange.X <= hex->GetGridLocation().X && hex->GetGridLocation().X <= SouthDoorRange.Y) ||
				(entrance != REast && hex->Tags.Contains("W") && WestDoorRange.X <= hex->GetGridLocation().Y && hex->GetGridLocation().Y <= WestDoorRange.Y))
			{
				FVector location = FVector(0, 0, 0);
				FRotator rotation = FRotator(0, 0, 0);
				ATBS_Door* newDoor = GetWorld()->SpawnActor<ATBS_Door>(levelDataRow->room1_Door.Get(),
					location,
					rotation);
				newDoor->AttachToHex(hex);
				newDoor->Tags = hex->Tags;
			}
			// hex is not a door - spawn wall
			else
			{
				// check that at least one side is empty, to signify a wall
				if (bHasHexNW && bHasHexNE && bHasHexE && bHasHexSE && bHasHexSW && bHasHexW);
				else
				{
					// spawn a wall on the Hex, Full or Half dependant on which sides are blank
					FVector location = FVector(0, 0, 0);
					FRotator rotation = FRotator(0, 0, 0);
					ATBS_Wall* newWall;
					if      (hex->GetRoomLevel() == Level1) newWall = GetWorld()->SpawnActor<ATBS_Wall>(levelDataRow->room1_WallFull.Get(), location, rotation);
					else if (hex->GetRoomLevel() == Level2) newWall = GetWorld()->SpawnActor<ATBS_Wall>(levelDataRow->room2_WallFull.Get(), location, rotation);
					else if (hex->GetRoomLevel() == Level3) newWall = GetWorld()->SpawnActor<ATBS_Wall>(levelDataRow->room3_WallFull.Get(), location, rotation);
					else if (hex->GetRoomLevel() == Level4) newWall = GetWorld()->SpawnActor<ATBS_Wall>(levelDataRow->room4_WallFull.Get(), location, rotation);
					else if (hex->GetRoomLevel() == Level5) newWall = GetWorld()->SpawnActor<ATBS_Wall>(levelDataRow->room4_WallFull.Get(), location, rotation);
					else                                    newWall = GetWorld()->SpawnActor<ATBS_Wall>(levelDataRow->room1_WallFull.Get(), location, rotation);
					newWall->AttachToHex(hex);
				}
				
			}
		}
	}
}

FIntPoint ATBS_GridSystem::GetDoorRange(int max, TEnumAsByte<RoomLevel> level)
{
	FIntPoint returnPoint = FIntPoint(-1,-1);
	if (level == Level1) // door size all - 2 from edges
	{
		returnPoint.X = 2;
		returnPoint.Y = max - 3;
		return returnPoint;
	}
	if (level == Level2) // door size 3
	{
		returnPoint.X = FMath::FRandRange(1,max - 3);
		returnPoint.Y = returnPoint.X + 2;
		return returnPoint;
	}
	if (level == Level3) // door size 1
	{
		returnPoint.X = FMath::FRandRange(2, max - 2);
		returnPoint.Y = returnPoint.X;
		return returnPoint;
	}
	if (level == Level4) // door size 1, middle of room
	{
		return FIntPoint(max/2, max/2);
	}
	return returnPoint; // level failed ( return -1,-1 ) no door
}

TEnumAsByte<RoomLevel> ATBS_GridSystem::GetRandRoomLevel(TEnumAsByte<RoomLevel> currentLevel)
{
	if (currentLevel == Level0) return Level1;
	if (currentLevel == Level1)
	{
		if (FMath::FRand() * (15 - currentRoomCount) < 1 || currentRoomCount > 10) return Level2;
		return Level1;
	}
	if (currentLevel == Level2)
	{
		if (FMath::FRand() * (30 - currentRoomCount) < 1 || currentRoomCount > 15) return Level3;
		return Level2;
	}
	if (currentLevel == Level3) // door size 1
	{
		if (FMath::FRand() * (50 - currentRoomCount) < 1 || currentRoomCount > 20) return Level4;
		return Level3;
	}
	if (currentLevel == Level4)
	{
		return Level5;
	}
	return Level0;
}

void ATBS_GridSystem::DestroyOldRooms()
{
	// find all actors of hex
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBS_Hex::StaticClass(), FoundActors);
	for (AActor* Actor : FoundActors)
	{
		if (Cast<ATBS_Hex>(Actor)->GetGridRoom() < currentRoomCount - maxRoomsShownCount)
		{
			// Despawn hex if matching room number
			Cast<ATBS_Hex>(Actor)->Despawn();
		}
	}
}
