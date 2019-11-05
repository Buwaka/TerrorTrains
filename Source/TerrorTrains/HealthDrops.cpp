// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthDrops.h"
#include "PlayerCharacter.h"
#include "HealthComponent.h"

// Sets default values
AHealthDrops::AHealthDrops()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = DropMesh;
}

// Called when the game starts or when spawned
void AHealthDrops::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AHealthDrops::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHealthDrops::Activate(AActor* OtherActor, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		auto tempPlayer = Cast<APlayerCharacter>(OtherActor);
		tempPlayer->Health->AddHealth(HealthAmount);
		Destroy();
	}
}

