// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GenericPlatform/GenericPlatform.h"
#include "TankAimingComponent.generated.h"

//enum for aiming state
UENUM()
enum class EFiringStatus : uint8
{
	Reloading,
	Aiming,
	Locked,
	Empty
};

//forward declaration
class UTankBarrel; 
class UTankTurret;
class AProjectile;

//hold barrel's properties and elevate method
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TANKGAME_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTankAimingComponent();

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet);

	//refactoring
	//void SetBarrelReference(UTankBarrel* BarrelToSet);
	//void SetTurretReference(UTankTurret* TurretToSet);

	//void AimAt(FVector HitLocation, float LaunchSpeed);
	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = Setup)
	void Fire();

	EFiringStatus GetFiringStatus() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = State)
	EFiringStatus FiringStatus = EFiringStatus::Reloading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = State)
	int32 AmmoCount = 40;

	//set this UPROPERTY because of a UE4 bug where this value was reset to NONE instead of Projectile_BP. Needs to be in the protected: category
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Setup)
	TSubclassOf<AProjectile> ProjectileBlueprint;

private:

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsBarrelMoving();

	void MoveBarrelToward(FVector AimDir);

	FVector AimDirection;

	UTankBarrel* Barrel = nullptr;
	UTankTurret* Turret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float LaunchSpeed = 100000; //starting value of 1000m/s

	//TODO declare function to rotate turret
	//void RotateTurretToward(FVector AimDirection);

	UPROPERTY(EditDefaultsOnly, Category = Firing)
	float ReloadTimeInSeconds = 3.0;

	double LastFireTime = 0.0;
};
