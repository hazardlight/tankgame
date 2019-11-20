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

protected:
	//How close the AI Tank can get to the player
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float AcceptanceRadius = 7000.0;

private:
	void BeginPlay() override;

	virtual void SetPawn(APawn* InPawn) override;

	UFUNCTION()
	void OnPossessedTankDeath();
	
	virtual void Tick(float DeltaTime) override;

	//ATank* GetControlledTank() const;

	//ATank* GetPlayerTank() const;
};
