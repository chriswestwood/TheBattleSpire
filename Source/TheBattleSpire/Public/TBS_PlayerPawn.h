// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "TBS_PlayerPawn.generated.h"

UCLASS()
class THEBATTLESPIRE_API ATBS_PlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATBS_PlayerPawn();

	// Returns cameraBoom
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return cameraBoom; }
	// Returns playerCamera
	FORCEINLINE class UCameraComponent* GetPlayerCamera() const { return playerCamera; }



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// input actions
	void MoveForward(float v);
	void MoveRight(float v);
	void RotateClock();
	void RotateAntiClock();
	UFUNCTION()
	void RotateProgress(float Value);

	/** Camera boom positioning the camera above the map */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* cameraBoom;
	/** player Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* playerCamera;

	//player movement
	float moveSpeed; //camera movement speed
	float baseTurnRate; // camera turn speed
	float rotateAmount; // amount to rotate per button press
	UPROPERTY()
	FTimeline rotateTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
	class UCurveFloat* rotateCurveFloat;
	UPROPERTY()
	FRotator startRotation;
	UPROPERTY()
	FRotator targetRotation; // target for the timeline rotation
};