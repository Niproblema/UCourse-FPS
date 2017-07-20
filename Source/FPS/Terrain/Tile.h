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
		void PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn, float Radius);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	bool CanSpawnAtLocation(FVector Location, float Radius);
	bool FindEmptyLocation(float Radius, FVector& OutSpawnPoint);
	void PlaceActor(TSubclassOf<AActor>ToSpawn, FVector SpawnPoint);
};
