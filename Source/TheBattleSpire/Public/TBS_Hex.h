// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/TimelineComponent.h"
#include "TBS_Hex.generated.h"

UCLASS()
class THEBATTLESPIRE_API ATBS_Hex : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBS_Hex();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGridLocation(FIntPoint newGridLoc);
	void SetGridRoom(int newRoomNum);
	const FIntPoint GetGridLocation();
	const int GetGridRoom();

	// moves the hex up and down on spawn/despawn
	void StartFloat();
	UFUNCTION()
	void FloatProgress(float Value);
	// called to despawn hex, used to also despawn any actor occuping
	void Drop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Hex Mesh Component
	UPROPERTY(VisibleDefaultsOnly, Category = Hex)
	class UStaticMeshComponent* hexMeshComp;
	// Hex Collision Component
	UPROPERTY(VisibleDefaultsOnly, Category = Hex)
	class USphereComponent* hexCollisionComp;

	// Grid Information
	FIntPoint gridLocation;
	int roomNumber;

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
