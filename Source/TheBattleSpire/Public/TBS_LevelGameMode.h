// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TBS_LevelGameMode.generated.h"

class ATBS_GridSystem;

/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API ATBS_LevelGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATBS_LevelGameMode();
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = grid)
	ATBS_GridSystem* GetGridSystem();

protected:

	ATBS_GridSystem* gridSystem;

	bool bIsPlayerTurn;

	bool bCanAction;
};
