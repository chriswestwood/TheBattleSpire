// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/TimelineComponent.h"
#include "EnumRoom.h"
#include "TBS_CameraPawn.generated.h"
class ATBS_Hex;

UCLASS()
class THEBATTLESPIRE_API ATBS_CameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATBS_CameraPawn();

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

	void UpdateParticle(ATBS_Hex* hex);
	void MoveRotationPlane(ATBS_Hex* hex);
	void DisableRotationPlane();
	void UpdateRotationPlane(TEnumAsByte<TileDirection> direction);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// input actions
	void MoveForward(float v);
	void MoveRight(float v);
	void MoveTo(FVector location);

	void RotateClock();
	void RotateAntiClock();

	UFUNCTION()
		void RotateProgress(float Value);
	UFUNCTION()
		void MoveProgress(float Value);

	// COMPONENTS
	// camera boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* cameraBoom;
	// camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* playerCamera;
	// Particle system for highlighted hex
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Particle, meta = (AllowPrivateAccess = "true"))
		class UParticleSystemComponent* hoverParticleComp;
	// light
	UPROPERTY(VisibleDefaultsOnly, Category = Pawn)
		class UPointLightComponent* handLight;
	// rotation plane
	UPROPERTY(VisibleDefaultsOnly, Category = Pawn)
		class UStaticMeshComponent* rotationPlane;

	//player movement

	// Rotation Timeline
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

	// Move Timeline
	float moveSpeed; //camera movement speed
	UPROPERTY()
		FTimeline moveTimeline;
	UPROPERTY(EditAnywhere, Category = "Timeline")
		class UCurveFloat* moveCurveFloat;
	UPROPERTY()
		FVector startLocation;
	UPROPERTY()
		FVector targetLocation; // target for the timeline move
};
