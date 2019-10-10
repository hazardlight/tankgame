// Fill out your copyright notice in the Description page of Project Settings.


#include "TankAimingComponent.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "Projectile.h"
#include "Math/Vector.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; //TODO Should this really tick?

	// ...
}
void UTankAimingComponent::BeginPlay()
{
	//so that the first time tanks can fire is after an initial reload
	LastFireTime = FPlatformTime::Seconds();

}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (AmmoCount <= 0)
	{
		FiringStatus = EFiringStatus::Empty;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringStatus = EFiringStatus::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringStatus = EFiringStatus::Aiming;
	}
	else
	{
		FiringStatus = EFiringStatus::Locked;
	}
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	auto BarrelVector = Barrel->GetForwardVector();

	return !BarrelVector.Equals(AimDirection, 0.01);
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
		AimDirection = OutLaunchVelocity.GetSafeNormal();
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

void UTankAimingComponent::MoveBarrelToward(FVector AimDir)
{
	if (!ensure(Barrel && Turret)) { return; }

	//Work out difference between current barrel rotation and AimDirection
	auto BarrelRotator = Barrel->GetForwardVector().Rotation(); //x axis
	auto AimAsRotator = AimDir.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;

	//UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *DeltaRotator.ToString())

	Barrel->Elevate(DeltaRotator.Pitch);

	//Turret->Rotate(DeltaRotator.Yaw);

	//this does not fix the aiming problem when aiming at -180 deg. The coordinate plane should be relative and not absolute
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}
/*
	void UTankAimingComponent::Reload() 
	{
		//Load an ammo type for the main gun
	}
*/

/*
void UTankAimingComponent::SelectAmmo() 
{
	//Selects the ammo to be loaded for the next shot
	//Requires time for the autoloader to ready the round depending on where it is in the carousel
}
*/

void UTankAimingComponent::Fire()
{
	if ( FiringStatus != EFiringStatus::Reloading && AmmoCount != 0 )
	{
		//spawn a projectile at the socket location on the barrel
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		AmmoCount--;
	}
}
EFiringStatus UTankAimingComponent::GetFiringStatus() const
{
	return FiringStatus;
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