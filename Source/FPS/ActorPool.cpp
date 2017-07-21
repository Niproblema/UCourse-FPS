// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "Containers/Queue.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

AActor * UActorPool::Checkout() {
	if (Pool.IsEmpty()) {
		return nullptr;
	}
	AActor* Rtn;
	Pool.Dequeue(Rtn);
	return Rtn;
}

void UActorPool::ReturnActor(AActor * ActorToReturn) {
	if (!ActorToReturn) return;
	Pool.Enqueue(ActorToReturn);
}

void UActorPool::PopulatePool(AActor * NewActor) {
	ReturnActor(NewActor);
}
