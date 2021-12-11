// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TBS_CharacterPawn.h"
#include "TBS_GameInstance.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStruct
{
	GENERATED_USTRUCT_BODY()

public:
	FCharacterStruct() {}
	FCharacterStruct(TSubclassOf<ATBS_CharacterPawn> newChar)
	{
		characterClass = newChar;
	};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Type)
	TSubclassOf<ATBS_CharacterPawn> characterClass;
};

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
	FCharacterStruct* GetCharacter(int num);
	int GetCharacterCount();
	
protected:

	FName currentLevelName;
	TArray<FCharacterStruct> characterStats;

	
};
