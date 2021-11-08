// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TBS_GameInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API UTBS_GameInstance : public UGameInstance
{
	GENERATED_BODY()

		UTBS_GameInstance();

public:

	FName getCurrentLevelName();
	void setCurrentLevelName(FString name);

protected:

	FName currentLevelName;

	
};
