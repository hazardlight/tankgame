// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "TankAimingComponent.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	/*
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
	*/
}
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto AITank = GetPawn();
	auto AimingComponent = AITank->FindComponentByClass<UTankAimingComponent>();
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	//if (GetPlayerTank())
	if (!ensure(PlayerTank && AITank)) { return; }
	
	//move toward player
	MoveToActor(PlayerTank, AcceptanceRadius);

	//Aim toward the player
	//GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	//GetControlledTank()->Fire();

	AimingComponent->Fire(); //TODO don't fire every frame
	
}

/*
ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}
*/

/*
ATank* ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
	{
		return nullptr;
	}
	return Cast<ATank>(PlayerPawn);
}
*/

