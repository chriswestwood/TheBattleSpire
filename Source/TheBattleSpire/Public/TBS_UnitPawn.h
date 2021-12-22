// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBS_Object.h"
#include "EnumRoom.h"
#include "TBS_UnitPawn.generated.h"
class UTBS_StatBarComponent;

UCLASS()
class THEBATTLESPIRE_API ATBS_UnitPawn : public ATBS_Object
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATBS_UnitPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool MoveToHex(ATBS_Hex* moveHex, TEnumAsByte<TileDirection> newDirection = TWest);
	TEnumAsByte<TileDirection> GetDirection();
	void SetDirection(TEnumAsByte<TileDirection> newDir);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Components
	// Unit
	UPROPERTY(VisibleDefaultsOnly, Category = Pawn)
	class UStaticMeshComponent* UnitMeshComp;
	UPROPERTY(VisibleDefaultsOnly, Category = Pawn)
	class UTBS_StatBarComponent* statusBar;

	UPROPERTY()
		TEnumAsByte<TileDirection> direction;

};
