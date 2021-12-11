// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GameInstance.h"

UTBS_GameInstance::UTBS_GameInstance()
{
	setCurrentLevelName("OrcVillage");
	static ConstructorHelpers::FClassFinder<ATBS_CharacterPawn> mageClass(TEXT("/Game/Blueprints/Player/TBS_MagePawn"));
	if (mageClass.Class)
	{
		characterStats.Add(FCharacterStruct(mageClass.Class));
	}
	static ConstructorHelpers::FClassFinder<ATBS_CharacterPawn> clericClass(TEXT("/Game/Blueprints/Player/TBS_ClericPawn"));
	if (clericClass.Class)
	{
		characterStats.Add(FCharacterStruct(clericClass.Class));
	}
	static ConstructorHelpers::FClassFinder<ATBS_CharacterPawn> fighterClass(TEXT("/Game/Blueprints/Player/TBS_FighterPawn"));
	if (fighterClass.Class)
	{
		characterStats.Add(FCharacterStruct(fighterClass.Class));
	}
	static ConstructorHelpers::FClassFinder<ATBS_CharacterPawn> rangerClass(TEXT("/Game/Blueprints/Player/TBS_RangerPawn"));
	if (rangerClass.Class)
	{
		characterStats.Add(FCharacterStruct(rangerClass.Class));
	}
}

FName UTBS_GameInstance::getCurrentLevelName()
{
	return currentLevelName;
}

void UTBS_GameInstance::setCurrentLevelName(FString name)
{
	currentLevelName = FName(name);
}

FCharacterStruct* UTBS_GameInstance::GetCharacter(int num)
{
	if (characterStats.Num() > num && num >= 0) return &characterStats[num];
	return nullptr;
}

int UTBS_GameInstance::GetCharacterCount()
{
	return characterStats.Num();
}
