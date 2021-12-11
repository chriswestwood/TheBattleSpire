// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_PlayerController.h"
#include "TBS_Object.h"
#include "TBS_Hex.h"
#include "TBS_CharacterPawn.h"
#include "TBS_CameraPawn.h"
#include "TBS_EnemyPawn.h"
#include "TBS_GameInstance.h"
#include "Card.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Particles/ParticleSystem.h"

ATBS_PlayerController::ATBS_PlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;
	DefaultMouseCursor = EMouseCursor::Hand;
	currentState = StateSpawn;
	bGameStart = false;
	mouseCoord = FVector(0, 0, 0);
}

void ATBS_PlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	// trace to mouse coord
	ATBS_Hex* traceHex = NULL;
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_GameTraceChannel1,false, Hit);
	if (Hit.bBlockingHit) {
		if (Hit.Actor != NULL) 
		{
			mouseCoord = Hit.Location;
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
	if(!UpdateHoverHex(traceHex)) return;
	// update HUD with selected hexes info
	// HUD->UpdateSelected(hoverHex)

	hoverActionHexes.Empty();
	// if a character pawn is selected (and on action) draw path towards hoverHex
	if (hoverHex && selectedPawn && currentState == StateAbility)
	{
		if (!selectedCard) return;
		if (selectedCard->selection == SDirection)
		{
			hoverActionHexes = selectedPawn->GetHex()->GetHexesDirection(hoverHex->GetActorLocation(), selectedCard->range);
		}
		else if (selectedCard->selection == SPoint)
		{
			//check if hoverHex is in range, else get closest that is 
			hoverActionHexes.Add(hoverHex);
		}
		else if (selectedCard->selection == SPath)
		{
			hoverActionHexes = selectedPawn->GetHex()->GetHexesPath(hoverHex->GetActorLocation(),selectedCard->range);
		}
	}
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
	//check if selected card


	// check if selected hex
	if (!hoverHex) return;
	if (currentState == StateSpawn); // select has no feature before game start
	else if (currentState == StateExplore || currentState == StateSelect)
	{
		// if explore or select state - check for player pawn and set to selected
		ATBS_CharacterPawn* selectedChar = Cast<ATBS_CharacterPawn>(hoverHex->GetOccupant());
		if (selectedChar)selectedPawn = selectedChar;
		// update the HUD to show new selected hex/player pawn

	}
	else if (currentState == StateAbility)
	{
		

	}
	ChangeActionState();
}

void ATBS_PlayerController::ClickAction()
{
	if (!hoverHex) return;
	ATBS_Object* clickObject = hoverHex->GetOccupant();
	if (currentState == StateSpawn)
	{
		if (hoverHex->ActorHasTag("Spawn") && !clickObject)
		{
			UTBS_GameInstance* GI = Cast<UTBS_GameInstance>(GetGameInstance());
			ATBS_CharacterPawn* newCharacter = GetWorld()->SpawnActor<ATBS_CharacterPawn>(GI->GetCharacter(characters.Num())->characterClass,FVector(0,0,0), FRotator(0,0,0));
			characters.Add(newCharacter);
			newCharacter->MoveToHex(hoverHex);
		}
	}
	if (currentState == StateSelect)
	{
		if (selectedPawn)
		{
			if (clickObject)
			{
				if (clickObject->Action(selectedPawn))
				{

				}
			}
			else
			{
				selectedPawn->MoveToHex(hoverHex);
			}
		}
	}
	if (currentState == StateAbility)
	{

	}
	if (currentState == StateExplore)
	{
		if (!selectedPawn) return;
		if (clickObject)
		{

		}
		else
		{
			selectedPawn->MoveToHex(hoverHex);
		}
	}
	ChangeActionState();
}

// Called to bind functionality to input
void ATBS_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// This is initialized on startup, you can go straight to binding
	InputComponent->BindAction("Select", IE_Pressed, this, &ATBS_PlayerController::ClickSelect);
	InputComponent->BindAction("Action", IE_Pressed, this, &ATBS_PlayerController::ClickAction);

}

bool ATBS_PlayerController::UpdateHoverHex(ATBS_Hex* hex)
{
	if (hex == hoverHex) return false;
	// new hex is highlighted
	hoverHex = hex;
	Cast<ATBS_CameraPawn>(GetPawn())->UpdateParticle(hoverHex);
	return true;
}

void ATBS_PlayerController::ChangeActionState()
{
	UTBS_GameInstance* GI = Cast<UTBS_GameInstance>(GetGameInstance());
	if (characters.Num() == GI->GetCharacterCount())
	{
		bGameStart = true;
	}
	if (!bGameStart)
	{
		currentState = StateSpawn;
		return;
	}
	// game has started, all player pawns have been spawned
	if (!CheckForEnemies())
	{
		currentState = StateExplore;
		return;
	}
	// enemies exist on level
	currentState = StateSelect;
	// if we have selected a pawn and have selected a card - enter ability state
	if (selectedCard && selectedPawn) currentState = StateAbility;
}

bool ATBS_PlayerController::CheckForEnemies()
{
	// return true if enemies are around
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATBS_EnemyPawn::StaticClass(), FoundActors);
	if (FoundActors.Num() == 0) return true;
	//else return false
	return false;
}
