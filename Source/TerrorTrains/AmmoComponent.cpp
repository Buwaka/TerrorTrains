// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UAmmoComponent::UAmmoComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAmmoComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	_Ammo = DefaultAmmoCount;
}


// Called every frame
void UAmmoComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UAmmoComponent::LoadAmmo(int amount)
{
	unsigned int temp =(unsigned int) FMath::Max(amount, 0);

	if (temp <= _Ammo)
	{
		_Ammo -= temp;
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::FromInt(_Ammo));
		return true;
	}

	return false;
}

int UAmmoComponent::GetAmmo()
{
	return _Ammo;
}

void UAmmoComponent::AddAmmo(int amount)
{
	unsigned int temp = (unsigned int)FMath::Max(amount, 0);
	_Ammo += temp;
}

void UAmmoComponent::RemoveAmmo(int amount)
{
	if ((unsigned int)std::abs(amount) > _Ammo)
	{
		_Ammo = 0;
	}
	else
	{
		_Ammo -= amount;
	}
}

void UAmmoComponent::SetAmmo(int amount)
{
	unsigned int temp = (unsigned int)FMath::Max(amount, 0);
	_Ammo = temp;
}

