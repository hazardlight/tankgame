// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false; //TODO Should this really tick?

	// ...
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

/* refactoring
void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!BarrelToSet)
	{
		return;
	}
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!TurretToSet)
	{
		return;
	}
	//TODO Set Turret Reference
	Turret = TurretToSet;
}
*/


void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel && Turret)) { return; } //exit if no barrel or turret reference found

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
		//RotateTurretToward(AimDirection); //turret rotates if fire solution is valid??
		MoveBarrelToward(AimDirection);
		auto Time = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f: Aim Solution Found"), Time)
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		//UE_LOG(LogTemp, Warning, TEXT("%f: No Aim Solution Found"), Time)
	}

	//auto OurTankName = GetOwner()->GetName();
	//auto BarrelLocation = Barrel->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s. LaunchSpeed: %f"), *OurTankName, *HitLocation.ToString(), *BarrelLocation.ToString(), LaunchSpeed)
}

void UTankAimingComponent::MoveBarrelToward(FVector AimDirection)
{
	if (!ensure(Barrel && Turret)) { return; }

	//Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation(); //x axis
	auto AimAsRotator = AimDirection.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *DeltaRotator.ToString())

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);

}
void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel && ProjectileBlueprint)) { return; }

	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;

	if (isReloaded)
	{
		//span a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
	}
}
/*
void UTankAimingComponent::RotateTurretToward(FVector AimDirection)
{
	//auto TurretRotator = Turret->GetUpVector().Rotation(); //need to get Z direction
	auto TurretRotator = Turret->GetForwardVector().Rotation(); //need to get Z direction

	auto AimAsRotator = AimDirection.Rotation();
	//auto DeltaRotator = FGenericPlatformMath::Abs<FRotator>(AimAsRotator) - TurretRotator;
	//auto DeltaRotator = AimAsRotator - TurretRotator;
	auto DeltaRotator = AimAsRotator - TurretRotator;



	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *AimAsRotator.ToString())


	Turret->Rotate(DeltaRotator.Yaw);


	//TODO work out difference between current turret rotation and AimDirection
}*/