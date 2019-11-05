// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include <string>

#include "TerrorTrains/PlayerCharacter.h"
#include "SFXComponent.h"
#include "TerrorTrainInstance.h"
#include "AmmoComponent.h"

#include "Engine/SkeletalMeshSocket.h"
#include "Engine/Engine.h"
#include "Engine/Classes/Animation/AnimSequence.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"


#define MARGIN 0.1f

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	Weapon->SetCollisionProfileName("WeaponCollision");
	OutOfAmmoSFX = CreateDefaultSubobject<USFXComponent>("SFX");
	RootComponent = Weapon;

	//default value in the instance where the socket doesn't have a relative position
	_RelBulletLocation = FVector(100, 0, 0);
	_Team = 0;
	_Timer = _RateOfFire;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	TerrorTrainInstance->GetSoundManager()->AddSound(this, GunShotSFX, Volume);

	_Damage = DefaultDamage;
	_RateOfFire = DefaultRateOfFire;
	_Speed = DefaultSpeed;
	_Spread = DefaultSpread;
	_BulletsPerShot = DefaultBulletsPerShot;
	_CostPerBullet = DefaultCostPerBullet;

	if (HipShootAnimation != NULL)
	{
		auto length = HipShootAnimation->GetPlayLength() + MARGIN;
		Weapon->SetPlayRate(_RateOfFire / length);
	}



	SetSocket(BulletSocketName);

	//if (Stationary)
	//{
	//	auto temp = Cast<APlayerCharacter>(GetParentActor());
	//	if (temp != nullptr)
	//	{
	//		temp->Freeze(true);
	//	}
	//}
}

void AWeapon::Destroyed()
{
	Super::Destroyed();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_Timer += DeltaTime;

	if (_StartAnimCoolDown)
	{
		_AnimCoolDown -= DeltaTime;
		if (_AnimCoolDown <= 0)
		{
			_StartAnimCoolDown = false;
		}
	}
	//TerrorTrainInstance->GetDropManager()->SpawnDrop(this, GetActorLocation(), "HP");
}

bool AWeapon::Shoot(AActor* shootingPlayer)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(shootingPlayer);
	if (player == NULL || player == nullptr)
	{
		return false;
	}

	auto temp = player->GetComponentByClass(UAmmoComponent::StaticClass());

	if (temp == NULL || temp == nullptr)
	{
		return false;
	}
	 UAmmoComponent* ammo = Cast<UAmmoComponent>(temp);

	if ( _Timer > _RateOfFire)
	{
		bool shot = false;
		for (size_t i = 0; i < _BulletsPerShot; i++)
		{
			if (ammo->LoadAmmo(_CostPerBullet))
			{
				FActorSpawnParameters SpawnParams{};
				SpawnParams.Owner = this;
				SpawnParams.Instigator = this->GetInstigator();



				//spawning the bullet
				auto rotation = this->GetActorRotation();
				rotation.Pitch = 0;
				rotation.Roll = 0;
				auto relativeBulletLocation = rotation.RotateVector(_RelBulletLocation);
				auto bullet = GetWorld()->SpawnActor<ABullet>(Bullet, this->GetActorLocation() + relativeBulletLocation, rotation, SpawnParams);
				bullet->SetOwner(this);


				//setting team and speed
				bullet->SetTeam(player->GetTeam());
				bullet->SetSpeed(_Speed);
				//bullet spread
				FRotator Spread{ 0 };
				Spread.Yaw = FMath::RandRange(-_Spread, _Spread);
				bullet->SetSpread(Spread);

				//Direction in which the bullet will fly
				auto forward = GetActorForwardVector();
				//constraint the Z-axis (height)
				forward.Z = 0;

				bullet->Shoot(forward);
				TerrorTrainInstance->GetSoundManager()->Play(GunShotSFX);
				auto charController = Cast<APlayerController>(player->GetController());

				charController->ClientPlayForceFeedback(FireRumble,false,true,"GunRumble");
				player->IsShooting = true;
				shot = true;

				if (OverrideDamage)
				{
					bullet->SetDamage(_Damage);
				}
			}
			else
			{
				if (ParticleBP != NULL && _AnimCoolDown <= 0)
				{
					_StartAnimCoolDown = true;
					_AnimCoolDown = 2.5f;
					GetWorld()->SpawnActor<AActor>(ParticleBP, GetActorLocation(), GetActorRotation());
				}
				OutOfAmmoSFX->Play();
				player->SwitchWeapon(true);
			}

			
		}
		_Timer = 0;

		return shot;
	}

	return false;
}

int AWeapon::GetTeam()
{
	return _Team;
}

void AWeapon::SetTeam(int team)
{
	_Team = team;
}

float AWeapon::GetDamage()
{
	return _Damage;
}

void AWeapon::SetDamage(int damage)
{
	_Damage = damage;
}

float AWeapon::GetRateOfFire()
{
	return _RateOfFire;
}

void AWeapon::SetRateOfFire(int ROF)
{
	_RateOfFire = ROF;
}

float AWeapon::GetSpread()
{
	return _Spread;
}

void AWeapon::SetSpread(int spread)
{
	_Spread = spread;
}

void AWeapon::SetSocket(FName Socket)
{
	auto temp = Weapon->GetSocketByName(Socket);
	if (temp != NULL)
	{
		_RelBulletLocation = temp->RelativeLocation;
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("void AWeapon::SetNewSocket(FName Socket), failed to link socket, socket doesn't exist or name is incorrect"));
}

UAnimSequence * AWeapon::GetIdleAnimation()
{
	return IdleAnimation;
}

UAnimSequence * AWeapon::GetHipShootAnimation()
{
	return HipShootAnimation;
}

UAnimSequence * AWeapon::GetAimShootAnimation()
{
	return AimShootAnimation;
}

UAnimSequence * AWeapon::GetSwitchAnimation()
{
	return SwitchAnimation;
}

UAnimSequence * AWeapon::GetAimLoopAnimation()
{
	return AimLoopAnimation;
}

UAnimSequence* AWeapon::GetBeginAnimation()
{
	return BeginAnimation;
}

