// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_Object.h"
#include "TBS_Hex.h"

// Sets default values
ATBS_Object::ATBS_Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	if (!BaseMeshComp)
	{
		BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		BaseMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		BaseMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BaseMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BaseMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	}
}

// Called when the game starts or when spawned
void ATBS_Object::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATBS_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ATBS_Object::Action(ATBS_Object* pawn)
{
	return false;
}

void ATBS_Object::ShowInformation()
{
}

void ATBS_Object::Select()
{
}

void ATBS_Object::Deselect()
{
}

void ATBS_Object::AttachToHex(ATBS_Hex* hex)
{
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	AttachToComponent(hex->GetOccupantComponent(),
						FAttachmentTransformRules::KeepRelativeTransform);
	if (currentHex)
	{
		currentHex->SetOccupant(nullptr);
	}
	currentHex = hex;
	hex->SetOccupant(this);

}

ATBS_Hex* ATBS_Object::GetHex()
{
	return currentHex;
}

void ATBS_Object::Despawn()
{
	SetLifeSpan(5);
}

