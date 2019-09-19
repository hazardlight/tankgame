// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "tank.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

class ATank;

UCLASS()
class TANKGAME_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	void BeginPlay() override;

	//ATank* GetControlledTank() const;

	//ATank* GetPlayerTank() const;

	virtual void Tick(float DeltaTime) override;
};
