// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TBS_PlayerController.generated.h"

class ATBS_CharacterPawn;
class ATBS_Object;
/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API ATBS_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
		ATBS_PlayerController();

		// Sets
		void UpdateLastSelectedObject(ATBS_Object* o);
		void UpdateStatHUD();

		// Input
		void ClickSelect();
		void ClickAction();

		// Called to bind functionality to input
		virtual void SetupInputComponent() override;

protected:

	ATBS_CharacterPawn* selectedPawn;
	ATBS_Object* lastSelectedObject;

};

