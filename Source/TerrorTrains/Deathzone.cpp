// Fill out your copyright notice in the Description page of Project Settings.

#include "Deathzone.h"
#include "PlayerCharacter.h"
#include "HealthComponent.h"

// Sets default values
ADeathzone::ADeathzone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	collisionBox->OnComponentHit.AddDynamic(this, &ADeathzone::KillPlayer);
}

// Called when the game starts or when spawned
void ADeathzone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeathzone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeathzone::KillPlayer(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* collidingCharacter = Cast<APlayerCharacter>(OtherActor);
		collidingCharacter->Health->RemoveHealth(100,0,false);
		collidingCharacter->ActivateRespawn();
	}
}

