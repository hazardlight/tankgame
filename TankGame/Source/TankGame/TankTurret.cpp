// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"


void UTankTurret::Rotate(float RelativeSpeed)
{
	//TODO rotate the turret the right amount this frame
	auto ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0, 1.0);
	auto RotationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto RawNewRotation = RelativeRotation.Yaw + RotationChange;
	SetRelativeRotation(FRotator(0, RawNewRotation, 0));
}