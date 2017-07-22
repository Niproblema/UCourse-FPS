// Fill out your copyright notice in the Description page of Project Settings.

#include "Tile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "ActorPool.h"
#include "EngineUtils.h"
#include "AI/Navigation/NavigationSystem.h"


// Sets default values
ATile::ATile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATile::BeginPlay() {
	Super::BeginPlay();
}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	Super::EndPlay(EndPlayReason); //?
	Pool->ReturnActor(NavMeshBoundsVolume);
}

// Called every frame
void ATile::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}

void ATile::SetPool(UActorPool * Pool) {
	this->Pool = Pool;

	ActivateNavMesh(Pool);
}

//Get NavMesh, Check if it's valid, set it's Location
void ATile::ActivateNavMesh(UActorPool * Pool) {
	NavMeshBoundsVolume = Pool->Checkout();
	if (NavMeshBoundsVolume == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("NavMeshPool is out of NavMeshes!"))
			return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("%s checked out: (%s) at location %s"), *GetName(), *NavMeshBoundsVolume->GetName(), *((GetActorLocation() + FVector(2000, 0, 250)).ToString()))
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + FVector(2000, 0, 250)); //This should be chaanged if size chnages
	GetWorld()->GetNavigationSystem()->Build(); //TODO this is super slow method
	//NavMeshBoundsVolume->Build
}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn, float Radius, float MinScale, float MaxScale) {
	TArray<FSpawnOptions> SpawnPositions = GenerateSpawnPositions(MinNSpawn, MaxNSpawn, MinScale, MaxScale, Radius);

	for (FSpawnOptions Curr : SpawnPositions) {
		PlaceActor(ToSpawn, Curr);
	}
}

void ATile::PlaceAI(TSubclassOf<APawn> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn, float Radius) {
	TArray<FSpawnOptions> SpawnPositions = GenerateSpawnPositions(MinNSpawn, MaxNSpawn, 1, 1, Radius);

	for (FSpawnOptions Curr : SpawnPositions) {
		//This is Same as PlaceActor(), but modified for AI
		APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
		Spawned->SetActorRelativeLocation(Curr.Location);
		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
		Spawned->SetActorRotation(FRotator(0, Curr.Rotation, 0));
		Spawned->SpawnDefaultController();
		Spawned->Tags.Add(FName("Opponent"));
	}
}

TArray<FSpawnOptions> ATile::GenerateSpawnPositions(int32 MinNSpawn, int32 MaxNSpawn, float MinScale, float MaxScale, float Radius) {
	TArray<FSpawnOptions> SpawnPositions;
	int32 NSpawn = FMath::RandRange(MinNSpawn, MaxNSpawn);
	FVector SpawnPoint;
	for (int32 n = 0; n < NSpawn; n++) {
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(Radius * RandomScale, SpawnPoint)) {
			float Rotation = FMath::RandRange(-180.f, 180.f);
			FSpawnOptions Options = { SpawnPoint, Rotation, RandomScale };
			//PlaceActor(ToSpawn, Options);
			SpawnPositions.Add(Options);
		}
	}
	return SpawnPositions;
}

bool ATile::FindEmptyLocation(float Radius, FVector& OutSpawnPoint) {
	FBox Bounds = FBox(MinTile, MaxTile);
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

	//Uncomment for Debug capsule for props
	/*FColor ResultColor = Result ? FColor::Red : FColor::Green;
	DrawDebugCapsule(GetWorld(), GlobalLocation, 0, Radius, FQuat::Identity, ResultColor, true, 100);*/

	return !Result;
}

void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, const FSpawnOptions& SpawnOptions) {
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnOptions.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnOptions.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnOptions.Scale));
}


