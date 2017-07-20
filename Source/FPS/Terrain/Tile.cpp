// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"



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


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn, float Radius) {
	int32 NSpawn = FMath::RandRange(MinNSpawn, MaxNSpawn);
	FVector SpawnPoint;
	for (int32 n = 1; n < NSpawn; n++) {
		if (FindEmptyLocation(Radius, SpawnPoint)) {
			PlaceActor(ToSpawn, SpawnPoint);
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%s: %i x %s"), *this->GetName(), NSpawn, *ToSpawn->GetName())

}

bool ATile::FindEmptyLocation(float Radius, FVector& OutSpawnPoint) {
	FVector Min = FVector(0, -2000, 0);
	FVector Max = FVector(4000, 2000, 0);
	FBox Bounds = FBox(Min, Max);
	const int32 MAX_ATTEMPTS = 100;
	for (int32 i = 0; i < MAX_ATTEMPTS; i++) {
		FVector CandidateSpawnPoint = FMath::RandPointInBox(Bounds);
		if (CanSpawnAtLocation(CandidateSpawnPoint, Radius)) {
			OutSpawnPoint = CandidateSpawnPoint;
			return true;
		}
	}
	return false;
}

bool ATile::CanSpawnAtLocation(FVector Location, float Radius) {
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	FHitResult HitResult;
	bool Result = GetWorld()->SweepSingleByChannel(
		HitResult,
		GlobalLocation,
		GlobalLocation,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(Radius)
	);

	FColor ResultColor = Result ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);
	return !Result;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FVector SpawnPoint) {
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPoint);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
}


