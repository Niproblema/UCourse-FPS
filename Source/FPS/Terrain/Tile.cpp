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
}


void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn, float Radius, float MinScale, float MaxScale) {
	int32 NSpawn = FMath::RandRange(MinNSpawn, MaxNSpawn);
	FVector SpawnPoint;
	for (int32 n = 0; n < NSpawn; n++) {
		float RandomScale = FMath::RandRange(MinScale, MaxScale);
		if (FindEmptyLocation(Radius * RandomScale, SpawnPoint)) {
			float Rotation = FMath::RandRange(-180.f, 180.f);
			FSpawnOptions Options = { SpawnPoint, Rotation, RandomScale };
			PlaceProp(ToSpawn, Options);
		}
	}
	//Uncomment to log out how many props per tile are about to be spawned
	//UE_LOG(LogTemp, Warning, TEXT("%s spawning: %i x %s"), *GetName(), NSpawn, *(ToSpawn->GetName()))
}

//Determines amount, finds space, rotation and places in world
void ATile::PlaceAI(TSubclassOf<APawn> ToSpawn, int32 MinNSpawn, int32 MaxNSpawn, float Radius) {
	int32 NSpawn = FMath::RandRange(MinNSpawn, MaxNSpawn); //Determine how many to spawn
	FVector SpawnPoint;
	for (int32 n = 0; n < NSpawn; n++) {
		if (FindEmptyLocation(Radius, SpawnPoint)) {
			float Rotation = FMath::RandRange(-180.f, 180.f);
			
			//This is Same as PlaceProp(), but modified for AI
			FRotator RRotator = FRotator(0, Rotation, 0);
			APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn, SpawnPoint, RRotator);
			if (!Spawned) {
				UE_LOG(LogTemp, Error, TEXT("%s Could not spawn Actor %s"), *GetName(), *(ToSpawn->GetName()))
					return;
			}
			Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
			Spawned->SpawnDefaultController();
			Spawned->Tags.Add(FName("Opponent"));
		}
	}
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

void ATile::PlaceProp(TSubclassOf<AActor> ToSpawn, const FSpawnOptions& SpawnOptions) {
	FRotator Rotation = FRotator(0, SpawnOptions.Rotation, 0);
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn, SpawnOptions.Location, Rotation);
	if (!Spawned) {
		UE_LOG(LogTemp, Error, TEXT("%s Could not spawn Actor %s"), *GetName(), *(ToSpawn->GetName()))
			return;
	}
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorScale3D(FVector(SpawnOptions.Scale));
}


