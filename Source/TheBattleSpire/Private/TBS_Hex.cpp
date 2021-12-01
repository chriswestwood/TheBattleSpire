// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_Hex.h"
#include "TBS_Object.h"
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

TArray<ATBS_Hex*> ATBS_Hex::GetHexDirection(TEnumAsByte<TileDirection> direction, int count)
{
	TArray<ATBS_Hex*> hexes;

	// line trace based on direction
	FHitResult Hit;
	FVector endLoc = GetActorLocation();
	// set the end location based on the direction
	if (direction == TileDirection::TNorthWest) { endLoc.X += 130; endLoc.Y += 225; }
	if (direction == TileDirection::TNorthEast) { endLoc.X -= 130; endLoc.Y += 225;	}
	if (direction == TileDirection::TEast) endLoc.X -= 260;
	if (direction == TileDirection::TSouthEast) { endLoc.X -= 130; endLoc.Y -= 225; }
	if (direction == TileDirection::TSouthWest) { endLoc.X += 130; endLoc.Y -= 225; }
	if (direction == TileDirection::TWest) endLoc.X += 260;

	// add this to the collision params so it does not hit itself
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	// get line trace to the next hex
	if (GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), endLoc, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams))
	{
		ATBS_Hex* nHex = Cast<ATBS_Hex>(Hit.GetActor()); // cast to hex
		if (nHex) // if valid
		{
			// call that hex get neighbour if we want more
			// add the hex to the output
			if(count > 1) hexes = nHex->GetHexDirection(direction, count - 1);
			hexes.Add(nHex);
		}
	}
	// return hex tiles if found
	return hexes;
}

TArray<ATBS_Hex*> ATBS_Hex::GetHexRadius(int size)
{
	return TArray<ATBS_Hex*>();
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
	hexCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	floatTimeline.ReverseFromEnd();
	SetLifeSpan(5);
}

