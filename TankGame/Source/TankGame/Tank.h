// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "TankAimingComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"


UCLASS()
class TANKGAME_API ATank : public APawn
{
	GENERATED_BODY()

public:
	
	//refactoring
	//UFUNCTION(BlueprintCallable, Category = Setup)
	//void SetBarrelReference(UTankBarrel* BarrelToSet);

	//UFUNCTION(BlueprintCallable, Category = Setup)
	//void SetTurretReference(UTankTurret* TurretToSet);

	//void AimAt(FVector HitLocation);

	//UFUNCTION(BlueprintCallable, Category = Setup)
	//void Fire();

	//refactoring
//protected:
	//UPROPERTY(BlueprintReadOnly)
	//UTankAimingComponent* TankAimingComponent = nullptr;

	//UPROPERTY(BlueprintReadOnly)
	//UTankMovementComponent* TankMovementComponent = nullptr;

private:
	// Sets default values for this pawn's properties
	ATank();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//refactoring
	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//TODO remove once firing is moved to AimingComponent
	//UPROPERTY(EditDefaultsOnly, Category = Firing)
	//float LaunchSpeed = 100000; //starting value of 1000m/s

	//UPROPERTY(EditDefaultsOnly, Category = Setup)
	//TSubclassOf<AProjectile> ProjectileBlueprint;

	//TODO Remove
	//UTankBarrel* Barrel = nullptr; //local barrel ref for spawning projectile

	//UPROPERTY(EditDefaultsOnly, Category = Firing)
	//float ReloadTimeInSeconds = 3.0;

	//double LastFireTime = 0.0;
};
