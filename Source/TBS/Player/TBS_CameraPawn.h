// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TBS_CameraPawn.generated.h"

UCLASS()
class TBS_API ATBS_CameraPawn : public APawn
{
	GENERATED_BODY()
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraints")
	float MaxZoom;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Constraints")
	float MinZoom;*/
public:
	// Sets default values for this pawn's properties
	ATBS_CameraPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	
};
