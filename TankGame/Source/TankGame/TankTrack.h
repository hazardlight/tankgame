// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * TankTrack is used to set the max driving force, to apply forces to the tank
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class TANKGAME_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	//Max force per track in newtons
	UPROPERTY(EditDefaultsOnly)
	float TrackMaxDrivingForce = 400000; //assume 40 tonne tank and 10m/s^2 acceleration
};
