// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_PlayerController.h"
#include "TBS_Object.h"
#include "TBS_Hex.h"
#include "TBS_CharacterPawn.h"

ATBS_PlayerController::ATBS_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Hand;
}

void ATBS_PlayerController::UpdateLastSelectedObject(ATBS_Object* o)
{
	// deselect current object
	if (lastSelectedObject == o) return;
	if (lastSelectedObject) lastSelectedObject->Deselect();
	// select new object
	lastSelectedObject = o;
	// if not null, perform the select function on object
	if (lastSelectedObject) lastSelectedObject->Select();
	// update the HUD with the new object
	UpdateStatHUD();
}

void ATBS_PlayerController::UpdateStatHUD()
{
}

void ATBS_PlayerController::ClickSelect()
{
	// trace to mouse coord
	ATBS_Object* selectedObject = NULL;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit) {
		if (Hit.Actor != NULL) {
			if (Cast<ATBS_Hex>(Hit.Actor)) selectedObject = Cast<ATBS_Hex>(Hit.Actor)->GetOccupant();
			else selectedObject = Cast<ATBS_Object>(Hit.Actor);
		}
	}
	// check for valid object
	if (selectedObject)
	{
		if (Cast<ATBS_CharacterPawn>(selectedObject)) selectedPawn = Cast<ATBS_CharacterPawn>(selectedObject);
	}
	UpdateLastSelectedObject(selectedObject);
}

void ATBS_PlayerController::ClickAction()
{
	// trace to mouse coord
	ATBS_Object* selectedObject = NULL;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	if (Hit.bBlockingHit) {
		if (Hit.Actor != NULL) {
			if (Cast<ATBS_Hex>(Hit.Actor)) selectedObject = Cast<ATBS_Hex>(Hit.Actor)->GetOccupant();
			else selectedObject = Cast<ATBS_Object>(Hit.Actor);
		}
	}
	if (selectedObject)
	{
		selectedObject->Action();
	}
}

// Called to bind functionality to input
void ATBS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// This is initialized on startup, you can go straight to binding
	InputComponent->BindAction("Select", IE_Pressed, this, &ATBS_PlayerController::ClickSelect);
	InputComponent->BindAction("Action", IE_Pressed, this, &ATBS_PlayerController::ClickAction);

}
