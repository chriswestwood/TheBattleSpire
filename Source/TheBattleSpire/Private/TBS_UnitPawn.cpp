// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_UnitPawn.h"
#include "TBS_Hex.h"

// Sets default values
ATBS_UnitPawn::ATBS_UnitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	BaseMeshComp;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObject(TEXT("StaticMesh'/Game/Assets/Models/SM_Base.SM_Base'"));
	if (MeshObject.Object)
	{
		BaseMeshComp->SetStaticMesh(MeshObject.Object);
		RootComponent = BaseMeshComp;
	}
	if (!UnitMeshComp)
	{
		// Use a sphere as a simple collision representation.
		UnitMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("UnitComponent"));
		UnitMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		UnitMeshComp->SetRelativeLocation(FVector(0, 0, 10));
	}
}

// Called when the game starts or when spawned
void ATBS_UnitPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBS_UnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATBS_UnitPawn::MoveToHex(ATBS_Hex* moveHex)
{
	if (moveHex->GetOccupant()) return false;
	AttachToHex(moveHex, true);
	return true;
}

