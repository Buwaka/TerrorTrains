// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponDrop.h"

#include "PlayerCharacter.h"

AWeaponDrop::AWeaponDrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = DropMesh;

}

// Called when the game starts or when spawned
void AWeaponDrop::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DropMesh->AddLocalRotation(FRotator(0, DeltaTime * 100, 0));
}

void AWeaponDrop::Activate(AActor* OtherActor, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		auto ptr = OtherActor->GetComponentByClass(UMoneyComponent::StaticClass());
		if (ptr != nullptr)
		{
			//ptr.addweapon(WeaponDrop);
		}
		Destroy();
	}
}

void AWeaponDrop::Randomize()
{
}
