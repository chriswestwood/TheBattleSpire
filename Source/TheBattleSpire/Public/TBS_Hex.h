// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "EnumRoom.h"
#include "TBS_Hex.generated.h"

class ATBS_Object;
class ATBS_Decoration;

UCLASS()
class THEBATTLESPIRE_API ATBS_Hex : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBS_Hex();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// grid functions
	void SetGridLocation(FIntPoint newGridLoc);
	void SetGridRoom(int newRoomNum);
	const FIntPoint GetGridLocation();
	const int GetGridRoom();
	const FVector GetBaseLocation();
	void SetRoomLevel(int newL);
	const int GetRoomLevel();

	// Neighbour functions
	TArray<ATBS_Hex*> GetHexDirection(TEnumAsByte<TileDirection> direction, int count);
	TArray<ATBS_Hex*> GetHexRadius(int size);

	// Occupant functions
	USceneComponent* GetOccupantComponent();
	ATBS_Object* GetOccupant();
	bool isOccupied();
	void SetOccupant(ATBS_Object* newOcc);
	void SetDecoration(ATBS_Object* newDec);

	// Called on creation to lift tile
	void Spawn();
	// timeline function
	UFUNCTION()
	void FloatProgress(float Value);
	// called to despawn hex, used to also despawn any actor occuping it as well
	void Despawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//COMPONENTS
	// Hex Mesh Component
	UPROPERTY(VisibleDefaultsOnly, Category = Hex)
	class UStaticMeshComponent* hexMeshComp;
	// Hex Collision Component
	UPROPERTY(VisibleDefaultsOnly, Category = Hex)
	class USphereComponent* hexCollisionComp;
	// Scene Component for occupant to stand
	UPROPERTY(VisibleDefaultsOnly, Category = Hex)
	class USceneComponent* occupantSceneComp;
	


	// Grid Information
	UPROPERTY(VisibleAnywhere, Category = Room)
	FIntPoint gridLocation;
	UPROPERTY(VisibleAnywhere, Category = Room)
	int roomNumber;
	int currentRoomLevel;

	//Occupant of hex
	UPROPERTY(VisibleAnywhere, Category = Room)
	ATBS_Object* occupant;
	// Decoration
	UPROPERTY(VisibleAnywhere, Category = Room)
	ATBS_Object* decoration;

	//spawn/despawn timeline info
	UPROPERTY()
	FTimeline floatTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* moveCurveFloat; // curve to smooth animation
	UPROPERTY()
	FVector startLocation; // starting location (0,0,-1000)
	UPROPERTY()
	FVector targetLocation; // target for the timeline move


};
