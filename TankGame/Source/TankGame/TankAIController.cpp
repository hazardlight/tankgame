// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	auto AITank = GetControlledTank();
	if (!AITank)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController not possessing a tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController possessing %s"), *(AITank->GetName()))
	}
	//UE_LOG(LogTemp, Warning, TEXT("TankAIController Begin Play"))

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController can't find PlayerTank"))

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TankAIController found player: %s"), *(PlayerTank->GetName()))

	}
}
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetPlayerTank())
	{
		//TODO move toward player

		//Aim toward the player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());

		//Fire if ready
	}
}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		return nullptr;
	}
	return Cast<ATank>(PlayerPawn);
}
