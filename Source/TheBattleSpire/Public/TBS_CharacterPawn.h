// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBS_UnitPawn.h"
#include "TBS_CharacterPawn.generated.h"

UCLASS()
class THEBATTLESPIRE_API ATBS_CharacterPawn : public ATBS_UnitPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATBS_CharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
