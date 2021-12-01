// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBS_PlayerController.generated.h"

class ATBS_CharacterPawn;
class ATBS_Object;
class ATBS_Hex;
class UParticleSystem;
/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API ATBS_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
		ATBS_PlayerController();
		virtual void PlayerTick(float DeltaTime) override;
		// Sets
		void UpdateSelectedObject(ATBS_Object* o);
		void UpdateStatHUD();

		// Input
		void ClickSelect();
		void ClickAction();

		// Called to bind functionality to input
		virtual void SetupInputComponent() override;

		//Updates
		void UpdateHoverHex(ATBS_Hex* hex);

protected:

	UPROPERTY()
	ATBS_CharacterPawn* selectedPawn;
	UPROPERTY()
	ATBS_Hex* selectedHex;
	UPROPERTY()
	ATBS_Hex* hoverHex;
	UPROPERTY()
	UParticleSystem* hoverParticle;

};

