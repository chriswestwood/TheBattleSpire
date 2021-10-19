// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_Hex.h"
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
		// Set the root component to be the collision component.
		RootComponent = hexCollisionComp;
	}
	if (!hexMeshComp)
	{
		hexMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		hexMeshComp->AttachToComponent(hexCollisionComp, FAttachmentTransformRules::KeepRelativeTransform);
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
		GetWorldTimerManager().SetTimer(UnusedHandle,this, &ATBS_Hex::StartFloat, FMath::FRand()/2, false);
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

void ATBS_Hex::StartFloat()
{
	floatTimeline.PlayFromStart();
}

void ATBS_Hex::FloatProgress(float Value)
{
	FVector newLocation = FMath::Lerp(startLocation, targetLocation, Value);
	hexMeshComp->SetRelativeLocation(newLocation, false);
}

void ATBS_Hex::Drop()
{
	floatTimeline.ReverseFromEnd();
	SetLifeSpan(5);
}

