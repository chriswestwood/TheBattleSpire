// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_Hex.h"
#include "TBS_Object.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"

// Sets default values
ATBS_Hex::ATBS_Hex()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!hexCollisionComp)
	{
		// Use a sphere as a simple collision representation.
		hexCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		// Set the sphere's collision radius.
		hexCollisionComp->InitSphereRadius(130.0f);
		// set trace response
		hexCollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		hexCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		hexCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
		hexCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Block);
		// Set the root component to be the collision component.
		RootComponent = hexCollisionComp;
	}
	if (!hexMeshComp)
	{
		hexMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		hexMeshComp->AttachToComponent(hexCollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
	if (!occupantSceneComp)
	{
		occupantSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("OccupantSceneComponent"));
		occupantSceneComp->AttachToComponent(hexMeshComp, FAttachmentTransformRules::KeepRelativeTransform);
	}
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HexSceneComponent"));
	}
	bisActive = true;
}

// Called when the game starts or when spawned
void ATBS_Hex::BeginPlay()
{
	Super::BeginPlay();
	if (moveCurveFloat)
	{
		startLocation = targetLocation = hexMeshComp->GetRelativeLocation();
		startLocation.Z += -2000.0f;
		targetLocation.Z += FMath::FRand() * 50.0f;
		hexMeshComp->SetRelativeLocation(startLocation, false);
		FOnTimelineFloat floatProgress;
		floatProgress.BindUFunction(this, FName("FloatProgress"));
		floatTimeline.AddInterpFloat(moveCurveFloat, floatProgress);
		floatTimeline.SetLooping(false);
		//StartFloat();
		FTimerHandle UnusedHandle;
		GetWorldTimerManager().SetTimer(UnusedHandle,this, &ATBS_Hex::Spawn, FMath::FRand()/2, false);
	}
}

// Called every frame
void ATBS_Hex::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	floatTimeline.TickTimeline(DeltaTime);
}

void ATBS_Hex::SetGridLocation(FIntPoint newGridLoc)
{
	gridLocation = newGridLoc;
}

void ATBS_Hex::SetGridRoom(int newRoomNum)
{
	roomNumber = newRoomNum;
}

const FIntPoint ATBS_Hex::GetGridLocation()
{
	return gridLocation;
}

const int ATBS_Hex::GetGridRoom()
{
	return roomNumber;
}

const FVector ATBS_Hex::GetBaseLocation()
{
	return targetLocation;
}

void ATBS_Hex::SetRoomLevel(int newL)
{
	currentRoomLevel = newL;
}

const int ATBS_Hex::GetRoomLevel()
{
	return currentRoomLevel;
}

TArray<ATBS_Hex*> ATBS_Hex::GetHexesDirection(FVector toward, int count, bool bOccupantBlock)
{
	TArray<ATBS_Hex*> hexes;
	FVector vectorAngle = toward - GetActorLocation();
	float angle = FMath::RadiansToDegrees(FMath::Atan2(vectorAngle.Y, vectorAngle.X));
	if (angle < 0) angle += 360;
	TEnumAsByte<TileDirection> direction;
	if (angle < 30 || angle > 330) direction = TWest;
	else if (angle < 90) direction = TNorthWest;
	else if (angle < 150) direction = TNorthEast;
	else if (angle < 210) direction = TEast;
	else if (angle < 270) direction = TSouthEast;
	else direction = TSouthWest;
	bool bEnd = false;
	while (!bEnd || hexes.Num() != count)
	{
		// get line trace to the next hex
		ATBS_Hex* nHex;
		if(hexes.Num() > 0) nHex = hexes.Last()->GetHexNeighbour(direction);
		else nHex = GetHexNeighbour(direction);
		if (nHex)
		{
				if (bOccupantBlock && nHex->isOccupied())
				{
					bEnd = true;
				}
				else
				{
					hexes.Add(nHex);
				}
		}
		else bEnd = true;
	}
	// return hex tiles if found
	return hexes;
}

TArray<ATBS_Hex*> ATBS_Hex::GetHexesRadius(int size, bool bOccupantBlock)
{
	return TArray<ATBS_Hex*>();
	// spheretracebychannel
}

TArray<ATBS_Hex*> ATBS_Hex::GetHexesPath(FVector endLoc,int count, bool bOccupantBlock)
{
	TArray<ATBS_Hex*> hexes;
	FHitResult Hit;
	hexes.Empty();
	// add this to the collision params so it does not hit itself
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	bool bEnd = false;
	FVector startLoc = GetActorLocation();
	while (!bEnd || hexes.Num() != count)
	{
		// get line trace to the next hex
		if (GetWorld()->LineTraceSingleByChannel(Hit, startLoc, endLoc, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
		{
			ATBS_Hex* nHex = Cast<ATBS_Hex>(Hit.GetActor()); // cast to hex
			if (nHex)
			{
				if (nHex->isActive())
				{
					if (bOccupantBlock && nHex->isOccupied())
					{
						bEnd = true;
					}
					else
					{
						hexes.Add(nHex);
						CollisionParams.AddIgnoredActor(nHex);
						startLoc = nHex->GetActorLocation();
					}
				}
				else bEnd = true;
			}
			else bEnd = true;
		}
		else bEnd = true;
	}
	return hexes;
}

bool ATBS_Hex::isHexInRange(ATBS_Hex* hex, int range)
{
	FVector distance = GetActorLocation() - hex->GetActorLocation();
	if (distance.Size() <= range * 260) return true;
	return false;
}

ATBS_Hex* ATBS_Hex::GetHexNeighbour(TileDirection direction)
{
	// line trace based on direction
	FHitResult Hit;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	FVector endLoc = GetActorLocation();
	FVector startLoc = GetActorLocation();
	// set the end location based on the direction
	if (direction == TileDirection::TNorthWest) { endLoc.X += 130; endLoc.Y += 225; }
	if (direction == TileDirection::TNorthEast) { endLoc.X -= 130; endLoc.Y += 225; }
	if (direction == TileDirection::TEast) endLoc.X -= 260;
	if (direction == TileDirection::TSouthEast) { endLoc.X -= 130; endLoc.Y -= 225; }
	if (direction == TileDirection::TSouthWest) { endLoc.X += 130; endLoc.Y -= 225; }
	if (direction == TileDirection::TWest) endLoc.X += 260;
	// get line trace to the next hex
	if (GetWorld()->LineTraceSingleByChannel(Hit, startLoc, endLoc, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
	{
		ATBS_Hex* nHex = Cast<ATBS_Hex>(Hit.GetActor()); // cast to hex
		if (nHex)
		{
			if (nHex->isActive())
			{
				return nHex;
			}
		}
	}	
	// return hex tiles if found
	return nullptr;
}

USceneComponent* ATBS_Hex::GetOccupantComponent()
{
	return occupantSceneComp;
}

ATBS_Object* ATBS_Hex::GetOccupant()
{
	return occupant;
}

bool ATBS_Hex::isOccupied()
{
	return occupant->IsValidLowLevel();
}

void ATBS_Hex::SetOccupant(ATBS_Object* newOcc)
{
	occupant = newOcc;
}

void ATBS_Hex::SetDecoration(ATBS_Object* newDec)
{
	decoration = newDec;
	decoration->AttachToComponent(GetOccupantComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);
}

bool ATBS_Hex::isActive()
{
	return bisActive;
}

void ATBS_Hex::Spawn()
{
	floatTimeline.PlayFromStart();
}

void ATBS_Hex::FloatProgress(float Value)
{
	FVector newLocation = FMath::Lerp(startLocation, targetLocation, Value);
	hexMeshComp->SetRelativeLocation(newLocation, false);
	if (Value > 1.5)
	{
		floatTimeline.Stop();
	}
}

void ATBS_Hex::Despawn()
{
	if (GetLifeSpan() != 0) return;
	if (occupant) occupant->Despawn();
	if (decoration) decoration->Despawn();
	bisActive = false;
	hexCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	floatTimeline.ReverseFromEnd();
	SetLifeSpan(5);
}

