// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_Object.h"

// Sets default values
ATBS_Object::ATBS_Object()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!BaseMeshComp)
	{
		BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		BaseMeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	}
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("BaseSceneComponent"));
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

