// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGun.h"
#include "Weapons/BaseProjectile.h"
#include "Animation/AnimInstance.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"


// Sets default values
ABaseGun::ABaseGun() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a gun mesh component
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	//GunMesh->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	//GunMesh->SetupAttachment(RootComponent);
	SetRootComponent(GunMesh);

	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMesh);
	//MuzzleLocation->SetRelativeLocation(FVector(0.2f, 53.4f, -10.6f));
}

// Called when the game starts or when spawned
void ABaseGun::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ABaseGun::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}


void ABaseGun::OnFire() {
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = MuzzleLocation->GetComponentRotation();
			// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
			const FVector SpawnLocation = MuzzleLocation->GetComponentLocation();

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<ABaseProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation1P != nullptr && AnimInstance1P != nullptr)
	{
		AnimInstance1P->Montage_Play(FireAnimation1P, 1.f);
	}
	if (FireAnimation3P != nullptr && AnimInstance3P != nullptr)
	{
		AnimInstance3P->Montage_Play(FireAnimation3P, 1.f);
	}
}

