// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GridSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TBS_Hex.h"
#include "TBS_Wall.h"
#include "TBS_Door.h"
#include "TBS_Object.h"
#include "RoomAssetStruct.h"
#include "TBS_GameInstance.h"

// Sets default values
ATBS_GridSystem::ATBS_GridSystem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableClass(TEXT("DataTable'/Game/Data/LevelAssetData.LevelAssetData'"));
	if (DataTableClass.Object)
	{
		levelDataTable = DataTableClass.Object;
	}
	static ConstructorHelpers::FObjectFinder<UDataTable> RoomDataTableClass(TEXT("DataTable'/Game/Data/RoomAssetData.RoomAssetData'"));
	if (RoomDataTableClass.Object)
	{
		roomDataTable = RoomDataTableClass.Object;
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
	if(GI && levelDataTable) levelDataRow = levelDataTable->FindRow<FLevelAssetStruct>(GI->getCurrentLevelName(), "");
	if (levelDataRow)
	{
		for (int x = 0; x < levelDataRow->room_Styles.Num(); x++)
		{
			roomDataRows.Add(roomDataTable->FindRow<FRoomAssetStruct>(levelDataRow->room_Styles[x], ""));
		}
		GenerateStartRoom();
	}
}

// Called every frame
void ATBS_GridSystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATBS_GridSystem::GenerateStartRoom()
{
	FRoomStruct startRoom = FRoomStruct();
	// setup the new room variables, add room to array.
	startRoom.roomSize = FIntPoint(7, 7);
	startRoom.roomWorldOffset = FIntPoint(0, 0);
	startRoom.roomNumber = 0;
	startRoom.roomLevel = 0;
	startRoom.roomNorthLevel = 1;
	startRoom.roomEastLevel = 1;
	startRoom.roomSouthLevel = 1;
	startRoom.roomWestLevel = 1;
	startRoom.entrance = RNone;
	startRoom.roomNorthDoorRange = GetDoorRange(startRoom.roomSize.X, roomDataRows[startRoom.roomNorthLevel]->room_Type);
	startRoom.roomEastDoorRange = GetDoorRange(startRoom.roomSize.Y, roomDataRows[startRoom.roomEastLevel]->room_Type);
	startRoom.roomSouthDoorRange = GetDoorRange(startRoom.roomSize.X, roomDataRows[startRoom.roomSouthLevel]->room_Type);
	startRoom.roomWestDoorRange = GetDoorRange(startRoom.roomSize.Y, roomDataRows[startRoom.roomWestLevel]->room_Type);

	// create new room
	startRoom.hexes = GenerateNewSquareRoom(startRoom, 0);
	GenerateWallsAndDoors(startRoom);
	//GenerateInterior(startRoom);

	// create spawn point and attach to middle hex
	int middleTileNum = startRoom.hexes.Num() % 2 > 0 ? startRoom.hexes.Num() / 2 : (startRoom.hexes.Num() % 2) + 1;
	ATBS_Object* spawnPoint = GetWorld()->SpawnActor<ATBS_Object>(roomDataRows[startRoom.roomLevel]->room_Objects[0].room_object,
		FVector(0, 0, 0), 
		FRotator(0, 0, 0));
	spawnPoint->AttachToHex(startRoom.hexes[middleTileNum], true);

	// set hexes around to spawn points
	ATBS_Hex* neighbour = startRoom.hexes[middleTileNum]->GetHexNeighbour(TEast);
	if (neighbour) neighbour->Tags.Add("Spawn");
	neighbour = startRoom.hexes[middleTileNum]->GetHexNeighbour(TNorthEast);
	if (neighbour) neighbour->Tags.Add("Spawn");
	neighbour = startRoom.hexes[middleTileNum]->GetHexNeighbour(TNorthWest);
	if (neighbour) neighbour->Tags.Add("Spawn");
	neighbour = startRoom.hexes[middleTileNum]->GetHexNeighbour(TWest);
	if (neighbour) neighbour->Tags.Add("Spawn");
	neighbour = startRoom.hexes[middleTileNum]->GetHexNeighbour(TSouthEast);
	if (neighbour) neighbour->Tags.Add("Spawn");
	neighbour = startRoom.hexes[middleTileNum]->GetHexNeighbour(TSouthWest);
	if (neighbour) neighbour->Tags.Add("Spawn");

	// Add room to array and update room counter
	Rooms.Add(startRoom);
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
		if (direction == RNorth && door->Tags.Contains("N")) door->Despawn();
		if (direction == REast && door->Tags.Contains("E")) door->Despawn();
		if (direction == RSouth && door->Tags.Contains("S")) door->Despawn();
		if (direction == RWest && door->Tags.Contains("W")) door->Despawn();
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
		newRoom.roomLevel = Rooms.Last().roomWestLevel;
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

	// get room size - random from level min/max values
	newRoom.roomSize.X = FMath::FRandRange(roomDataRows[newRoom.roomLevel]->roomSize.X, roomDataRows[newRoom.roomLevel]->roomSize.Y);
	newRoom.roomSize.Y = FMath::FRandRange(roomDataRows[newRoom.roomLevel]->roomSize.X, roomDataRows[newRoom.roomLevel]->roomSize.Y);

	// calc Door ranges
	newRoom.roomNorthDoorRange = GetDoorRange(newRoom.roomSize.X, roomDataRows[newRoom.roomNorthLevel]->room_Type);
	newRoom.roomEastDoorRange = GetDoorRange(newRoom.roomSize.Y, roomDataRows[newRoom.roomEastLevel]->room_Type);
	newRoom.roomSouthDoorRange = GetDoorRange(newRoom.roomSize.X, roomDataRows[newRoom.roomSouthLevel]->room_Type);
	newRoom.roomWestDoorRange = GetDoorRange(newRoom.roomSize.Y, roomDataRows[newRoom.roomWestLevel]->room_Type);
	
	// setup the new room variables, add room to array.
	newRoom.roomWorldOffset = LastWorldOffset;
	newRoom.roomNumber = currentRoomCount;
	if (direction == RNorth) newRoom.entrance = RSouth;
	else if (direction == RSouth) newRoom.entrance = RNorth;
	else if (direction == REast) newRoom.entrance = RWest;
	else if (direction == RWest) newRoom.entrance = REast;
	else newRoom.entrance = RNone;

	// destroy the oldest room so there is only a max of 2
	DestroyOldRooms();
	// create new room based on setup
	newRoom.hexes = GenerateNewSquareRoom(newRoom, doorOffset);
	GenerateWallsAndDoors(newRoom);
	GenerateInterior(newRoom);
	Rooms.Add(newRoom);
	currentRoomCount += 1;
}

TArray<ATBS_Hex*> ATBS_GridSystem::GenerateNewSquareRoom(struct FRoomStruct& room,
															int doorOffset)
{
	FIntPoint roomOffset;
	// calculate the room offset based on several factors
	// room needs to be connected to all doors from previous room
	// direction plays into which offset will be randomised
	//
	if (room.entrance == RSouth)
	{
		roomOffset.X = -room.roomSize.X + doorOffset + 4;
		roomOffset.Y = Rooms[Rooms.Num()-1].roomSize.Y;
	}
	else if (room.entrance == REast)
	{
		roomOffset.X = Rooms[Rooms.Num() - 1].roomSize.X;
		roomOffset.Y = -room.roomSize.Y + doorOffset + 4;
	}
	else if (room.entrance == RNorth)
	{
		roomOffset.X = -room.roomSize.X + doorOffset + 4;
		roomOffset.Y = -room.roomSize.Y;
	}
	else if (room.entrance == RWest)
	{
		roomOffset.X = -room.roomSize.X;
		roomOffset.Y = -room.roomSize.Y + doorOffset + 4;
	}
	else roomOffset = FIntPoint(0, 0);

	// add the room offset to the world offset, used to calculate the world positions
	LastWorldOffset.X += roomOffset.X;
	LastWorldOffset.Y += roomOffset.Y;

	// spawn hexes based on 2d array, size is dictated from roomSize.
	TArray<ATBS_Hex*> Hexes;
	// for each y
	for (int j = 0; j < room.roomSize.Y; ++j)
	{
		// for each x
		for (int i = 0; i < room.roomSize.X; ++i)
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
			ATBS_Hex* newHex = nullptr;
			TSubclassOf<ATBS_Hex> hexClass;
			// Spawn Hex based on where it is located in the grid
			if (i == 0) // x is 0 - East Side (X values go right to left)
			{
				hexClass = GetRandHex(roomDataRows[room.roomEastLevel]->room_Hexs);
				if (hexClass)
				{
					newHex = GetWorld()->SpawnActor<ATBS_Hex>(hexClass, FVector(x, y, 0), FRotator(0, 0, 0));
					newHex->Tags.Add("E");
					newHex->Tags.AddUnique("Edge");
					newHex->SetRoomLevel(room.roomEastLevel);
				}
			}
			else if (i == room.roomSize.X - 1) // x is max - West Side
			{
				hexClass = GetRandHex(roomDataRows[room.roomWestLevel]->room_Hexs);
				if (hexClass)
				{
					newHex = GetWorld()->SpawnActor<ATBS_Hex>(hexClass, FVector(x, y, 0), FRotator(0, 0, 0));
					newHex->Tags.Add("W");
					newHex->Tags.AddUnique("Edge");
					newHex->SetRoomLevel(room.roomWestLevel);
				}
			}
			else if (j == 0) // y is 0 - South Side
			{
				hexClass = GetRandHex(roomDataRows[room.roomSouthLevel]->room_Hexs);
				if (hexClass)
				{
					newHex = GetWorld()->SpawnActor<ATBS_Hex>(hexClass, FVector(x, y, 0), FRotator(0, 0, 0));
					newHex->Tags.Add("S");
					newHex->Tags.AddUnique("Edge");
					newHex->SetRoomLevel(room.roomSouthLevel);
				}
			}
			else if (j == room.roomSize.Y - 1) // y is max - North Side
			{
				hexClass = GetRandHex(roomDataRows[room.roomNorthLevel]->room_Hexs);
				if (hexClass)
				{
					newHex = GetWorld()->SpawnActor<ATBS_Hex>(hexClass, FVector(x, y, 0), FRotator(0, 0, 0));
					newHex->Tags.Add("N");
					newHex->Tags.AddUnique("Edge");
					newHex->SetRoomLevel(room.roomNorthLevel);
				}
			}
			else
			{
				hexClass = GetRandHex(roomDataRows[room.roomLevel]->room_Hexs);
				if (hexClass) 
				{
					newHex = GetWorld()->SpawnActor<ATBS_Hex>(hexClass, FVector(x, y, 0), FRotator(0, 0, 0));
					newHex->Tags.Add("Room");
					newHex->SetRoomLevel(room.roomLevel);
				}
			}	

			if (IsValid(newHex))
			{
				newHex->SetGridLocation(FIntPoint(i, j));
				newHex->SetGridRoom(currentRoomCount);
				Hexes.Add(newHex);				
			}
				
		}
	}
	return Hexes;
}

void ATBS_GridSystem::GenerateWallsAndDoors(struct FRoomStruct& room)
{
	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), ATBS_Hex::StaticClass(), "Edge", FoundActors);
	for (AActor* Actor : FoundActors)
	{
		ATBS_Hex* hex = Cast<ATBS_Hex>(Actor);
		if (hex && hex->GetGridRoom() == currentRoomCount)
		{
			bool bHasHexNW = IsValid(hex->GetHexNeighbour(TNorthWest));
			bool bHasHexNE = IsValid(hex->GetHexNeighbour(TNorthEast));
			bool bHasHexE = IsValid(hex->GetHexNeighbour(TEast));
			bool bHasHexSE = IsValid(hex->GetHexNeighbour(TSouthEast));
			bool bHasHexSW = IsValid(hex->GetHexNeighbour(TSouthWest));
			bool bHasHexW = IsValid(hex->GetHexNeighbour(TWest));
			// check for door, ensure the entrance wall does not have a door added
			// door is added if hex pos is within the range found above
			if ((room.entrance != RNorth && hex->Tags.Contains("N") && room.roomNorthDoorRange.X <= hex->GetGridLocation().X && hex->GetGridLocation().X <= room.roomNorthDoorRange.Y) ||
				(room.entrance != REast && hex->Tags.Contains("E") && room.roomEastDoorRange.X <= hex->GetGridLocation().Y && hex->GetGridLocation().Y <= room.roomEastDoorRange.Y) ||
				(room.entrance != RSouth && hex->Tags.Contains("S") && room.roomSouthDoorRange.X <= hex->GetGridLocation().X && hex->GetGridLocation().X <= room.roomSouthDoorRange.Y) ||
				(room.entrance != RWest && hex->Tags.Contains("W") && room.roomWestDoorRange.X <= hex->GetGridLocation().Y && hex->GetGridLocation().Y <= room.roomWestDoorRange.Y))
			{
				FVector location = FVector(0, 0, 0);
				FRotator rotation = FRotator(0, 0, 0); // calculate based on missing neighbours
				ATBS_Door* newDoor = GetWorld()->SpawnActor<ATBS_Door>(GetRandObject(roomDataRows[hex->GetRoomLevel()]->room_Doors),
					location,
					rotation);
				newDoor->AttachToHex(hex, true);
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
					newWall = GetWorld()->SpawnActor<ATBS_Wall>(GetRandObject(roomDataRows[hex->GetRoomLevel()]->room_FullWalls), location, rotation);
					newWall->AttachToHex(hex, true);
				}				
			}
		}
	}
}

void ATBS_GridSystem::GenerateInterior(FRoomStruct& room)
{
	// add decorations to the floor
	for (ATBS_Hex* hex : room.hexes)
	{
		if(hex->Tags.Contains("Edge")); // dont spawn on edge tiles
		else
		{
			//create decoration based on room type
			TSubclassOf<ATBS_Object> decorationClass = GetRandObject(roomDataRows[room.roomLevel]->room_Decorations);
			if (decorationClass)
			{
				ATBS_Object* decoration = GetWorld()->SpawnActor<ATBS_Object>(decorationClass, FVector(0, 0, 0), FRotator(0, 0, 0));
				hex->SetDecoration(decoration);
			}
		}
	}
	// add objects in the room, this includes things like barrels/traps/chests/blockables
	int spawnedCountLeft = FMath::FRandRange(roomDataRows[room.roomLevel]->objectCount.X,roomDataRows[room.roomLevel]->objectCount.Y);
	while (spawnedCountLeft >0)
	{
		TSubclassOf<ATBS_Object> objectClass = GetRandObject(roomDataRows[room.roomLevel]->room_Objects,true);
		if (objectClass != NULL)
		{
			ATBS_Object* newObject = GetWorld()->SpawnActor<ATBS_Object>(objectClass, FVector(0, 0, 0), FRotator(0, 0, 0));
			ATBS_Hex* randHex = room.hexes[FMath::FRandRange(0, room.hexes.Num() - 1)];
			while (randHex->isOccupied())
			{
				randHex = room.hexes[FMath::FRandRange(0, room.hexes.Num() - 1)];
			}
			newObject->AttachToHex(randHex, true);
		}
		spawnedCountLeft--;
	}




	// spawn enemies in the room



}

FIntPoint ATBS_GridSystem::GetDoorRange(int max, TEnumAsByte<RoomType> type)
{
	FIntPoint returnPoint;
	if (type == Type1) // door size all - 2 from edges
	{
		return FIntPoint(2, max - 3);
	}
	else if (type == Type2) // door size 3
	{
		returnPoint.X = FMath::FRandRange(1,max - 3);
		returnPoint.Y = returnPoint.X + 2;
		return returnPoint;
	}
	else if (type == Type3) // door size 1
	{
		returnPoint.X = FMath::FRandRange(2, max - 2);
		returnPoint.Y = returnPoint.X;
		return returnPoint;
	}
	else return FIntPoint(-1, -1); // type is none - output no doors (-1,-1)
}

int ATBS_GridSystem::GetRandRoomLevel(int currentRoomLevel)
{
	if (currentRoomLevel == 0) return 1;
	if (currentRoomLevel == roomDataRows.Num()) return -1;
	if (FMath::FRand() * (10 * currentRoomLevel - currentRoomCount) < 1 || currentRoomCount > 10 * currentRoomLevel) return currentRoomLevel + 1;
	return currentRoomLevel;
}

TSubclassOf<ATBS_Hex> ATBS_GridSystem::GetRandHex(TArray<FHexPercentage> Hexes)
{
	float chance = FMath::FRand() * 100;
	for (int i = 0; i < Hexes.Num(); i++)
	{
		if (chance < Hexes[i].percentage) return Hexes[i].room_hex;
	}
	return Hexes.Last().room_hex;
}

TSubclassOf<ATBS_Object> ATBS_GridSystem::GetRandObject(TArray<FObjectPercentage> Objects, bool returnLastOnNone)
{
	float chance = FMath::FRand() * 100;
	for (int i = 0; i < Objects.Num(); i++)
	{
		if (chance < Objects[i].percentage) return Objects[i].room_object;
	}
	if (returnLastOnNone && Objects.Num() > 0) return Objects.Last().room_object;
	return NULL;
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
