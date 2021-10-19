// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_PlayerPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATBS_PlayerPawn::ATBS_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("PlayerSceneComponent"));
	}
	// Create a camera boom
	if (!cameraBoom)
	{
		cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
		cameraBoom->SetupAttachment(RootComponent);
		cameraBoom->AddLocalOffset(FVector(0, 0, 0), false);
		cameraBoom->SocketOffset = FVector(0, 0, 600);
		cameraBoom->TargetArmLength = 600.0f; // The camera follows at this distance behind the character	
		cameraBoom->bUsePawnControlRotation = false; // Rotate the arm based on the controller
		cameraBoom->bInheritPitch = true;
		cameraBoom->bInheritYaw = true;
		cameraBoom->bInheritRoll = true;
		cameraBoom->bDoCollisionTest = false;
	}
	// Create a camera
	playerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	playerCamera->SetupAttachment(cameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	playerCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	playerCamera->SetRelativeRotation(FRotator(-30, 0, 0));

	moveSpeed = 15.0f;
	baseTurnRate = 45.f;
	rotateAmount = 60.0f;
}


// Called when the game starts or when spawned
void ATBS_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	targetRotation = GetActorRotation();
	if (rotateCurveFloat)
	{
		FOnTimelineFloat rotateProgress;
		rotateProgress.BindUFunction(this, FName("RotateProgress"));
		rotateTimeline.AddInterpFloat(rotateCurveFloat, rotateProgress);
		rotateTimeline.SetLooping(false);
	}
}

// Called every frame
void ATBS_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	rotateTimeline.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void ATBS_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &ATBS_PlayerPawn::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ATBS_PlayerPawn::MoveRight);
	PlayerInputComponent->BindAction("RotateClock", EInputEvent::IE_Pressed, this, &ATBS_PlayerPawn::RotateClock);
	PlayerInputComponent->BindAction("RotateAntiClock", EInputEvent::IE_Pressed,this, &ATBS_PlayerPawn::RotateAntiClock);
}

void ATBS_PlayerPawn::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FVector direction = UKismetMathLibrary::GetForwardVector(GetActorRotation());
		AddActorWorldOffset(direction.operator*(Value * moveSpeed), false);
	}
}

void ATBS_PlayerPawn::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		FVector direction = UKismetMathLibrary::GetRightVector(GetActorRotation());
		AddActorWorldOffset(direction.operator*(Value * moveSpeed), false);
	}
}

void ATBS_PlayerPawn::RotateClock()
{
	//AddActorWorldRotation(FRotator(0, 90, 0), false);
	if (rotateCurveFloat)
	{
		startRotation = GetActorRotation();
		targetRotation.Yaw += rotateAmount;
		rotateTimeline.PlayFromStart();
	}
	
}
void ATBS_PlayerPawn::RotateAntiClock()
{
	//AddActorWorldRotation(FRotator(0, -90, 0), false);
	if (rotateCurveFloat)
	{
		startRotation = GetActorRotation();
		targetRotation.Yaw += -rotateAmount;
		rotateTimeline.PlayFromStart();
	}
}

void ATBS_PlayerPawn::RotateProgress(float Value)
{
	FRotator newRotation = FMath::Lerp(startRotation, targetRotation, Value);
	RootComponent->SetWorldRotation(newRotation, false);

}

