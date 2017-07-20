// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class FPS_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = Gameplay)
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn = 1, int32 MaxNSpawn = 1, float Radius = 300, float MinScale = 1, float MaxScale = 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool CanSpawnAtLocation(FVector Location, float Radius);
	bool FindEmptyLocation(float Radius, FVector& OutSpawnPoint);
	void PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint, float Rotation, float Scale);

	const int32 MAX_ATTEMPTS = 100;
};
