// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TBS_Object.generated.h"
class ATBS_UnitPawn;
class ATBS_Hex;

UCLASS()
class THEBATTLESPIRE_API ATBS_Object : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATBS_Object();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// player interaction
	virtual bool Action(ATBS_UnitPawn* pawn);
	virtual void ShowInformation(); // shows the selection info on HUD
	virtual void Select();
	virtual void Deselect();
	// Hex functions
	void AttachToHex(ATBS_Hex* hex, bool MoveTo = false);
	ATBS_Hex* GetHex();
	void DetachFromHex();

	// Spawn functions
	void Despawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	// Base
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	class UStaticMeshComponent* BaseMeshComp;

	// Object Stats
	int Health; // at 0 object is destroyed
	bool bTargetable; // can be selected as a target for target actions
	bool bDamageable; // can be damaged by attacks
	bool bUseable; // can be used when in range
	ATBS_Hex* currentHex; // space it occupies
};
