// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_PlayerController.h"
#include "TBS_Object.h"
#include "TBS_Hex.h"
#include "TBS_CharacterPawn.h"
#include "TBS_CameraPawn.h"
#include "Particles/ParticleSystem.h"

ATBS_PlayerController::ATBS_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Hand;
}

void ATBS_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// trace to mouse coord
	ATBS_Hex* traceHex = NULL;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1, false, Hit);
	if (Hit.bBlockingHit) {
		if (Hit.Actor != NULL) 
		{
			ATBS_Object* traceObject = Cast<ATBS_Object>(Hit.Actor);
			if (traceObject)
			{
				traceHex = traceObject->GetHex();
			}
			else
			{
				traceHex = Cast<ATBS_Hex>(Hit.Actor);
			}
		}
	}
	UpdateHoverHex(traceHex);
}

void ATBS_PlayerController::UpdateSelectedObject(ATBS_Object* o)
{
	// deselect current object
	//if (SelectedObject == o) return;
	//if (SelectedObject) lastSelectedObject->Deselect();
	// select new object
	//lastSelectedObject = o;
	// if not null, perform the select function on object
	//if (lastSelectedObject) lastSelectedObject->Select();
	// update the HUD with the new object
	//UpdateStatHUD();
}

void ATBS_PlayerController::UpdateStatHUD()
{
}

void ATBS_PlayerController::ClickSelect()
{
	if (hoverHex)
	{

	}
}

void ATBS_PlayerController::ClickAction()
{
	if (hoverHex)
	{
		ATBS_Object* clickObject = NULL;
		clickObject = hoverHex->GetOccupant();
		if (clickObject)
		{
			if (clickObject->Action(selectedPawn)) 
			{

			}
		}
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

void ATBS_PlayerController::UpdateHoverHex(ATBS_Hex* hex)
{
	if (hex == hoverHex) return;
	// new hex is highlighted
	hoverHex = hex;
	Cast<ATBS_CameraPawn>(GetPawn())->UpdateParticle(hoverHex);
}
