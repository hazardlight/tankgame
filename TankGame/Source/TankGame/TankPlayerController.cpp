// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "Public/CollisionQueryParams.h"
#include "Tank.h"
#include "TankAimingComponent.h"



void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	auto AimingComponent = GetControlledTank()->FindComponentByClass<UTankAimingComponent>();

	if (AimingComponent)
	{
		FoundAimingComponent(AimingComponent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController can't find AimingComponent at BeginPlay"))
	}
	/*
	auto ControlledTank = GetControlledTank();
	if (!ControlledTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController not possessing a tank"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerController possessing %s"), *(ControlledTank->GetName()))
	}
	*/
	//UE_LOG(LogTemp, Warning, TEXT("PlayerController Begin Play"))


}

void ATankPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Warning, TEXT("PlayerController is Ticking"))

	AimTowardCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const 
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardCrosshair()
{
	if (!GetControlledTank()) 
	{
		return;
	}

	FVector HitLocation; //Out Parameter

	//is going to line trace
	if (GetSightRayHitLocation(HitLocation)) //if the line trace hits the landscape
	{
		GetControlledTank()->AimAt(HitLocation);
	}
	
}
//get world location of the linetrace through the crosshair, return true if it hits the landscape
bool ATankPlayerController::GetSightRayHitLocation(FVector& HitLocation) const
{
	bool ValidHit = true;

	//Find the Crosshair position in px coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewportSizeY * CrossHairYLocation);

		//this also works for ScreenLocation:
		//float Xoff = ViewportSizeX * CrossHairXLocation;
		//float Yoff = ViewportSizeY * CrossHairYLocation;
		//FVector2D ScreenLocation(Xoff, Yoff);

	//De-project screen position of the crosshair to a world direction

	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		//line trace along that look direction and see what we hit up to some max range
		GetLookVectorHitLocation(LookDirection, HitLocation);
		//UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *HitLocation.ToString())

	}

	return ValidHit;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; //to be discarded
	return DeprojectScreenPositionToWorld(
			ScreenLocation.X, 
			ScreenLocation.Y, 
			CameraWorldLocation, 
			LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	
	if (GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation,
				ECollisionChannel::ECC_Visibility)
			)
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0);
	return false;
}