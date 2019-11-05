// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoDrop.h"
#include "AmmoComponent.h"
#include "PlayerCharacter.h"

AAmmoDrop::AAmmoDrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = DropMesh;
	Cost->SetMoney(0);
}

// Called when the game starts or when spawned
void AAmmoDrop::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAmmoDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoDrop::Activate(AActor* OtherActor, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		auto ptr = OtherActor->GetComponentByClass(UAmmoComponent::StaticClass());
		if (ptr != nullptr)
		{
			auto ammoComp = Cast<UAmmoComponent>(ptr);
			if (ammoComp != nullptr)
			{
				ammoComp->AddAmmo(AmmoAmount);
			}
		}
		Destroy();
	}
}