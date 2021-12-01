// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TBS_Object.h"
#include "TBS_Door.generated.h"
class ATBS_Object;

/**
 * 
 */
UCLASS()
class THEBATTLESPIRE_API ATBS_Door : public ATBS_Object
{
	GENERATED_BODY()
	
public:
	ATBS_Door();
	bool Action(ATBS_Object* pawn) override;

	void DeactivateDoor();

protected:

	bool bDoorActive;

};
