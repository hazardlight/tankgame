// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"
#include "Math/UnrealMathUtility.h"

void UTankBarrel::Elevate(float RelativeSpeed)
{
	//move the barrel the right amount this frame
	//given a max elevation speed and the frame rate
	auto ClampedRelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1.0, 1.0);
	auto ElevationChange = ClampedRelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;

	auto RawNewElevation = RelativeRotation.Pitch + ElevationChange;
	auto ClampedElevation = FMath::Clamp<float>(RawNewElevation, MinElevationDegrees, MaxElevationDegrees);

	SetRelativeRotation(FRotator(ClampedElevation, 0, 0));

	//SetRelativeRotation(FRotator(RawNewElevation, 0, 0));

}