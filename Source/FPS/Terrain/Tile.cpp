// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"


// Sets default values
ATile::ATile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay() {
	Super::BeginPlay();

}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn) {
	int32 NSpawn = FMath::RandRange(MinNSpawn, MaxNSpawn);
	FVector Min = FVector(0, -2000, 0);
	FVector Max = FVector(4000, 2000, 0);
	FBox Bounds = FBox(Min, Max);
	for (int32 n = 1; n < NSpawn; n++) {
		FVector SpawnPoint = FMath::RandPointInBox(Bounds);
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
		Spawned->SetActorRelativeLocation(SpawnPoint);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));	
	}
	UE_LOG(LogTemp, Warning, TEXT("%s: %i x %s"), *this->GetName(), NSpawn, *ToSpawn->GetName())

}


