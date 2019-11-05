// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeWeapon.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "TrainWalls.h"
#include "TrainWagon.h"
#include "TerrorTrainInstance.h"

// Sets default values
AMeleeWeapon::AMeleeWeapon()
{

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	//Weapon->SetCollisionProfileName("WeaponCollision");
	//RootComponent = Weapon;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	RootComponent = Mesh;
	_Swing = false;

	//_Team = 0;
	//_Timer = _RateOfFire;

	//WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	//Mesh->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnHit);
}

void AMeleeWeapon::BeginPlay()
{	
	Super::BeginPlay();
}


// Called every frame
void AMeleeWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_Timer += DeltaTime;

}

bool AMeleeWeapon::Shoot(AActor* shootingPlayer)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(shootingPlayer);
	if (player == NULL || player == nullptr)
	{
		return false;
	}

	if (_Timer > _RateOfFire)
	{
		player->IsHitting = true;
		_Swing = true;
		_Timer = 0;
		return true;
	}
	return false;
}

void AMeleeWeapon::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && _Swing)
	{
		auto ptr = OtherActor->GetComponentByClass(UHealthComponent::StaticClass());
		if (ptr != nullptr)
		{
			auto healthComp = Cast<UHealthComponent>(ptr);
			if (healthComp->GetTeam() != _Team)
			{
				healthComp->RemoveHealth(_Damage, _Team);
				TerrorTrainInstance->GetSoundManager()->Play(GunShotSFX);
			}
		}
		_Swing = false;
	}
}

void AMeleeWeapon::SetWeaponMeshOnIndex(int index)
{
	auto children  = RootComponent->GetAttachChildren();
	for (auto child : children)
	{
		if (child->IsA(UStaticMeshComponent::StaticClass()))
		{
			auto num = pMeshes.Add(Cast<UStaticMeshComponent>(child));
			pMeshes[num]->SetVisibility(false);
		}
	}
	pMeshes[index]->SetVisibility(true);
	pMeshes[index]->OnComponentBeginOverlap.AddDynamic(this, &AMeleeWeapon::OnHit);
	Mesh = pMeshes[index];
	for (auto mesh : pMeshes)
	{
		if (mesh != Mesh)
		{
			mesh->DestroyComponent();
		}
	}

}


