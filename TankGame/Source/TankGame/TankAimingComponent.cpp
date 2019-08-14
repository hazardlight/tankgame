// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; //TODO Should this really tick?

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	//TODO Set Turret Reference
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) { return; }

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	//Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.0,
		0.0,
		ESuggestProjVelocityTraceOption::DoNotTrace //parameter must be present to prevent bug
	);
	
	if(bHaveAimSolution)
	{
		auto OurTankName = GetOwner()->GetName();
		auto AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelToward(AimDirection);
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: Aim Solution Found"), Time)
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: No Aim Solution Found"), Time)
	}

	//auto OurTankName = GetOwner()->GetName();
	//auto BarrelLocation = Barrel->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s. LaunchSpeed: %f"), *OurTankName, *HitLocation.ToString(), *BarrelLocation.ToString(), LaunchSpeed)
}

void UTankAimingComponent::MoveBarrelToward(FVector AimDirection)
{
	//Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation(); //x axis
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *DeltaRotator.ToString())

	Barrel->Elevate(DeltaRotator.Pitch);
}
void UTankAimingComponent::RotateTurretToward(FVector AimDirection)
{
	//TODO work out difference between current turret rotation and AimDirection
}