// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnOptions {
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;

UCLASS()
class FPS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();

	UFUNCTION(BlueprintCallable, Category = Spawning)
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn = 1, int32 MaxNSpawn = 1, float Radius = 300, float MinScale = 1, float MaxScale = 1);

	UFUNCTION(BlueprintCallable, Category = Spawning)
		void PlaceAI(TSubclassOf<APawn> ToSpawn, int32 MinNSpawn = 1, int32 MaxNSpawn = 1, float Radius = 90);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = Pool)
		void SetPool(UActorPool * Pool);

	UPROPERTY(EditDefaultsOnly, Category = Setup)
		FVector MinTile = FVector(0, -2000, 0);
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		FVector MaxTile = FVector(4000, 2000, 0);

private:
	bool CanSpawnAtLocation(FVector Location, float Radius);
	bool FindEmptyLocation(float Radius, FVector& OutSpawnPoint);
	void PlaceProp(TSubclassOf<AActor>ToSpawn, const FSpawnOptions& SpawnOptions);
	void ActivateNavMesh(UActorPool * Pool);

	UActorPool * Pool = nullptr;
	AActor * NavMeshBoundsVolume = nullptr;
	const int32 MAX_ATTEMPTS = 50;
};
