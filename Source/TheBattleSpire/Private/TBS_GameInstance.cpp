// Fill out your copyright notice in the Description page of Project Settings.


#include "TBS_GameInstance.h"

UTBS_GameInstance::UTBS_GameInstance()
{
	setCurrentLevelName("OrcVillage");
}

FName UTBS_GameInstance::getCurrentLevelName()
{
	return currentLevelName;
}

void UTBS_GameInstance::setCurrentLevelName(FString name)
{
	currentLevelName = FName(name);
}
