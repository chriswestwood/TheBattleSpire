// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBS_PlayerController.generated.h"

class ATBS_CharacterPawn;
class ATBS_Object;
class ATBS_Hex;
class UParticleSystem;
class UCard;

UENUM()
enum StateEnum
{
	StateSpawn		UMETA(DisplayName = "Spawn"),
	StateSelect		UMETA(DisplayName = "Select"),
	StateAbility	UMETA(DisplayName = "Ability"),
	StateExplore	UMETA(DisplayName = "Explore")
};

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
		bool UpdateHoverHex(ATBS_Hex* hex);
		void ChangeActionState();
		bool CheckForEnemies();

		UPROPERTY()
			FVector mouseCoord;
protected:

	UPROPERTY()
	TArray<ATBS_CharacterPawn*> characters;
	UPROPERTY()
	ATBS_CharacterPawn* selectedPawn;
	UPROPERTY()
	TArray<ATBS_Hex*> currentActionHexes;
	UPROPERTY()
	TArray<ATBS_Hex*> hoverActionHexes;
	UPROPERTY()
	UCard* selectedCard;
	UPROPERTY()
	ATBS_Hex* hoverHex;
	UPROPERTY()
	UParticleSystem* hoverParticle;
	UPROPERTY()
	TEnumAsByte<StateEnum> currentState;

	UPROPERTY()
		bool bGameStart;


};

