// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseGun.generated.h"

UCLASS()
class FPS_API ABaseGun : public AActor
{
	GENERATED_BODY()

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USkeletalMeshComponent* GunMesh;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USceneComponent* MuzzleLocation;

public:
	// Sets default values for this actor's properties
	ABaseGun();
	UFUNCTION(BlueprintCallable, Category = Gameplay)
	void OnFire();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ABaseProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation1P;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		class UAnimMontage* FireAnimation3P;


	UPROPERTY()
		class UAnimInstance* AnimInstance1P;
	UPROPERTY()
		class UAnimInstance* AnimInstance3P;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

};
