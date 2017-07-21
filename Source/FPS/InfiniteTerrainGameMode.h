// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPSGameMode.h"
#include "InfiniteTerrainGameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPS_API AInfiniteTerrainGameMode : public AFPSGameMode
{
	GENERATED_BODY()
	
public:
	AInfiniteTerrainGameMode();

	UFUNCTION(BlueprintCallable, Category = ActorPools)
		void PopulateBoundsVolumePool();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = ActorPools)
		class UActorPool * NavMeshBoundsVolumePool;
private:
	void AddToPool(class ANavMeshBoundsVolume* VolumeToAdd);

	
};
