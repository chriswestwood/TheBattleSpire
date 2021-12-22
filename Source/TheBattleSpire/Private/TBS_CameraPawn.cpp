// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_CameraPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TBS_Hex.h"
#include "TBS_PlayerController.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ATBS_CameraPawn::ATBS_CameraPawn()
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
	if(!hoverParticleComp)
	{
		hoverParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HoverParticle"));
		hoverParticleComp->SetupAttachment(RootComponent);
		UParticleSystem* defaultParticleSystem;
		static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleClass(TEXT("ParticleSystem'/Game/Blueprints/Player/Particles/DefaultHoverParticle.DefaultHoverParticle'"));
		if (ParticleClass.Object)
		{
			defaultParticleSystem = ParticleClass.Object;
			hoverParticleComp->SetTemplate(defaultParticleSystem);
		}
	}
	if (!handLight)
	{
		handLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
		handLight->Intensity = 1000.0f;
	}
	if (!rotationPlane)
	{
			rotationPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RotationPlane"));
			static ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("StaticMesh'/Game/Assets/Models/SM_Arrow.SM_Arrow'"));
			if(MeshAsset.Object)rotationPlane->SetStaticMesh(MeshAsset.Object);
			rotationPlane->SetVisibility(false);
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
void ATBS_CameraPawn::BeginPlay()
{
	Super::BeginPlay();
	hoverParticleComp->DeactivateSystem();
	targetRotation = GetActorRotation();
	if (rotateCurveFloat)
	{
		FOnTimelineFloat rotateProgress;
		rotateProgress.BindUFunction(this, FName("RotateProgress"));
		rotateTimeline.AddInterpFloat(rotateCurveFloat, rotateProgress);
		rotateTimeline.SetLooping(false);
	}
	targetLocation = GetActorLocation();
	if (moveCurveFloat)
	{
		FOnTimelineFloat moveProgress;
		moveProgress.BindUFunction(this, FName("MoveProgress"));
		moveTimeline.AddInterpFloat(moveCurveFloat, moveProgress);
		moveTimeline.SetLooping(false);
	}
}

// Called every frame
void ATBS_CameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	rotateTimeline.TickTimeline(DeltaTime);
	moveTimeline.TickTimeline(DeltaTime);
	ATBS_PlayerController* PC = Cast<ATBS_PlayerController>(GetController());
	if(PC) handLight->SetWorldLocation(PC->mouseCoord);
}

void ATBS_CameraPawn::UpdateParticle(ATBS_Hex* hex)
{
	if (hex)
	{
		if (hex->GetLifeSpan() == 0)
		{
			if (!hoverParticleComp->IsActive()) hoverParticleComp->Activate();
			hoverParticleComp->AttachToComponent(hex->GetOccupantComp(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			// set particle type based on hex
			return;
		}
	}
	hoverParticleComp->Deactivate();

}

void ATBS_CameraPawn::MoveRotationPlane(ATBS_Hex* hex)
{
	rotationPlane->SetVisibility(true);
	rotationPlane->SetWorldLocation(hex->GetActorLocation() + FVector(0, 0, 100));
}

void ATBS_CameraPawn::DisableRotationPlane()
{
	rotationPlane->SetVisibility(false);
}

void ATBS_CameraPawn::UpdateRotationPlane(TEnumAsByte<TileDirection> direction)
{
	if (direction == TNorthEast) rotationPlane->SetWorldRotation(FRotator(0, 120, 0));
	else if (direction == TEast) rotationPlane->SetWorldRotation(FRotator(0, 180, 0));
	else if (direction == TSouthEast) rotationPlane->SetWorldRotation(FRotator(0, 240, 0));
	else if (direction == TSouthWest) rotationPlane->SetWorldRotation(FRotator(0, 300, 0));
	else if (direction == TWest) rotationPlane->SetWorldRotation(FRotator(0, 0, 0));
	else if (direction == TNorthWest) rotationPlane->SetWorldRotation(FRotator(0, 60, 0));
}

// Called to bind functionality to input
void ATBS_CameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("Forward", this, &ATBS_CameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &ATBS_CameraPawn::MoveRight);
	PlayerInputComponent->BindAction("RotateClock", EInputEvent::IE_Pressed, this, &ATBS_CameraPawn::RotateClock);
	PlayerInputComponent->BindAction("RotateAntiClock", EInputEvent::IE_Pressed, this, &ATBS_CameraPawn::RotateAntiClock);
}

void ATBS_CameraPawn::MoveForward(float Value)
{
	//if ((Controller != nullptr) && (Value != 0.0f))
	//{
	//	FVector direction = UKismetMathLibrary::GetForwardVector(GetActorRotation());
	//	AddActorWorldOffset(direction.operator*(Value * moveSpeed), false);
	//}
	if (moveCurveFloat && (Value != 0.0f))
	{
		FVector direction = UKismetMathLibrary::GetForwardVector(GetActorRotation());
		startLocation = RootComponent->GetRelativeLocation();
		targetLocation += direction.operator*(Value * moveSpeed);
		moveTimeline.PlayFromStart();
	}
}

void ATBS_CameraPawn::MoveRight(float Value)
{
	if (moveCurveFloat && (Value != 0.0f))
	{
		FVector direction = UKismetMathLibrary::GetRightVector(GetActorRotation());
		startLocation = RootComponent->GetRelativeLocation();
		targetLocation += direction.operator*(Value * moveSpeed);
		moveTimeline.PlayFromStart();
	}
}

void ATBS_CameraPawn::MoveTo(FVector location)
{
	if (moveCurveFloat)
	{
		startLocation = RootComponent->GetRelativeLocation();
		targetLocation = location;
		targetLocation.Z = 0;
		moveTimeline.PlayFromStart();
	}
}

void ATBS_CameraPawn::RotateClock()
{
	//AddActorWorldRotation(FRotator(0, 90, 0), false);
	if (rotateCurveFloat)
	{
		startRotation = GetActorRotation();
		targetRotation.Yaw += rotateAmount;
		rotateTimeline.PlayFromStart();
	}

}
void ATBS_CameraPawn::RotateAntiClock()
{
	//AddActorWorldRotation(FRotator(0, -90, 0), false);
	if (rotateCurveFloat)
	{
		startRotation = GetActorRotation();
		targetRotation.Yaw += -rotateAmount;
		rotateTimeline.PlayFromStart();
	}
}

void ATBS_CameraPawn::RotateProgress(float Value)
{
	FRotator newRotation = FMath::Lerp(startRotation, targetRotation, Value);
	RootComponent->SetWorldRotation(newRotation, false);

}

void ATBS_CameraPawn::MoveProgress(float Value)
{
	FVector newLocation = FMath::Lerp(startLocation, targetLocation, Value);
	RootComponent->SetWorldLocation(newLocation, false);
}

