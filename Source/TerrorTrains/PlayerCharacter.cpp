// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "MoneyComponent.h"
#include "HealthComponent.h"
#include "AmmoComponent.h"
#include "SFXComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	Health = CreateDefaultSubobject<UHealthComponent>("Health");
	Money = CreateDefaultSubobject<UMoneyComponent>("Money");
	Ammo = CreateDefaultSubobject<UAmmoComponent>("Ammo");
	DamagedSFX = CreateDefaultSubobject<USFXComponent>("SFX");
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.0f, 0.f);
	GetCharacterMovement()->AirControl = 1.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;


	WeaponInventory = TMap<EWeaponType, TSubclassOf<class AWeapon>>();
	RootComponent = GetCapsuleComponent();

	
	/*if (RevolverBP != NULL)
	{
		PreviouwWeaponSwitchAnimation = RevolverBP.GetDefaultObject()->SwitchAnimation;
		_Weapon = RevolverBP.GetDefaultObject();
	}*/
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitGuns();

	if (WeaponInventory[EWeaponType::Revolver] != NULL)
	{
		GetGun(WeaponInventory[EWeaponType::Revolver]);
	}


}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (_IsDashing)
	//{
	//	CheckDash(DeltaTime);
	//}
	//if (_OnCooldown)
	//{
	//	CheckCooldown(DeltaTime);
	//}
	if (!_StartRespawnTimer)
	{
		CheckHealth();
		CheckMove();
	}
	else
	{
		_RespawnTimer -= DeltaTime;
		if (_RespawnTimer <= 0)
		{
			_Respawn = true;
			Health->ResetHealth();
			_StartRespawnTimer = false;
		}
	}

	//if (!_HasSetHealth)
	//{
	//	Health->SetTeam(_Team);
	//	_HasSetHealth = true;
	//}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!PlayerInputComponent)
	{
		return;
	}
	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis("RotateForward", this, &APlayerCharacter::RotateForward);
	PlayerInputComponent->BindAxis("RotateRight", this, &APlayerCharacter::RotateRight);
	PlayerInputComponent->BindAction("Fire", IE_Repeat, this, &APlayerCharacter::FireGun);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::FireGun);
	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &APlayerCharacter::SwitchWeapon<false>);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &APlayerCharacter::Interact);
	PlayerInputComponent->BindAction("Aim",IE_Pressed,this, &APlayerCharacter::SetAiming);
}

void APlayerCharacter::CheckMove()
{
	if (_MoveY == 0 && _MoveX == 0 && !_StartRespawnTimer)
	{
		IsWalking = false;
	}
	else
	{
		IsWalking = true;
	}
}


void APlayerCharacter::GetGun(TSubclassOf<class AWeapon> BP, TSubclassOf<class AActor> switchAnimationBP)
{
	IsHitting = false;
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };

	if(_Weapon != NULL)
	{//expliciete case voor anim BP
		PreviouwWeaponSwitchAnimation = _Weapon->SwitchAnimation;
	}
	else
	{
		PreviouwWeaponSwitchAnimation = BP.GetDefaultObject()->SwitchAnimation;
	}

	_Weapon = GetWorld()->SpawnActor<AWeapon>(BP, Location, Rotation, SpawnInfo);
	if (_Weapon->IsA(AMeleeWeapon::StaticClass()))
	{
		Cast<AMeleeWeapon>(_Weapon)->SetWeaponMeshOnIndex(_PlayerIndex);
	}
	_Weapon->AttachToComponent(GetMesh(), aRules, "Weapon_Socket");
	_Weapon->SetTeam(_Team);
	_Weapon->SetOwner(this);

	if (switchAnimationBP != NULL)
	{
		GetWorld()->SpawnActor<AActor>(switchAnimationBP, GetActorLocation(), GetActorRotation());
	}
}

void APlayerCharacter::RemoveCurrentGun()
{
	_Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	_Weapon->Destroy();
	_Weapon = nullptr;
}

void APlayerCharacter::InitGuns()
{
	
}

AWeapon * APlayerCharacter::GetWeapon()
{
	return _Weapon;
}


void APlayerCharacter::FireGun()
{
	if(_Weapon != nullptr && !_StartRespawnTimer)
	{
		if (!IsSwitchingWeapon)
		{
			if (_Weapon->Shoot(this))
			{
				if (_Weapon->IsA(AMeleeWeapon::StaticClass()))
				{
					IsHitting = true;
				}
				else
				{
					IsShooting = true;
				}
			}
		}
	}
}

//Move player in direction of input (Y-axis)
void APlayerCharacter::MoveForward(float Value)
{   
	if(!_StartRespawnTimer)
	{
		_MoveY = Value;
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation = FRotator{ 0, Rotation.Yaw, 0 };
		const FVector tDirection{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) };
		AddMovementInput(tDirection, Value);
	}
}

//Move player in direction of input (X-axis)
void APlayerCharacter::MoveRight(float Value)
{
	if(!_StartRespawnTimer)
	{
		_MoveX = Value;
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation = FRotator{ 0, Rotation.Yaw, 0 };
		const FVector tDirection{ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) };
		AddMovementInput(tDirection, Value);
	}
		
}

//Rotate in direction of input (Y-Axis)
void APlayerCharacter::RotateForward(float Value)
{
	if (Value != NULL)
	{
		_ValueY = Value;
		double Angle{};
		Angle = atan2(_ValueY, _ValueX);
		const FRotator yawRotation{0, float((Angle + 1.5708) * 180 / PI),0};
		SetActorRotation(yawRotation);
		Direction = GetActorRotation().Yaw;
	}
}

//Rotate in direction of input (X-Axis)
void APlayerCharacter::RotateRight(float Value)
{
	if (Value != NULL)
	{
		_ValueX = Value;
		double Angle{};
		Angle = atan2(_ValueY, _ValueX);
		const FRotator yawRotation{0, float((Angle + 1.5708) * 180 / PI), 0};
		SetActorRotation(yawRotation);
		Direction = GetActorRotation().Yaw;
	}
}

void APlayerCharacter::Interact()
{
	if(!_StartRespawnTimer)
	_CanInteract = true;
}

bool APlayerCharacter::WantsToInteract()
{
	if (_CanInteract  && !_StartRespawnTimer)
	{
		_CanInteract = false;
		return true;
	}
	return false;
}

void APlayerCharacter::SetAiming()
{
	if (!IsAiming)
	{
		IsAiming = true;
		SavedSpread = _Weapon->GetSpread();
		_Weapon->SetSpread(SavedSpread/2);
		GetCharacterMovement()->MaxWalkSpeed = 250;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 340.0f, 0.f); 
	}
	else
	{
		IsAiming = false;
		_Weapon->SetSpread(SavedSpread);
		GetCharacterMovement()->MaxWalkSpeed = 500;
		GetCharacterMovement()->RotationRate = FRotator(0.f, 540.0f, 0.f); 
	}
}

void APlayerCharacter::CheckHealth()
{
	if (Health->GetHealth() <= 0  && !_StartRespawnTimer)
	{
		auto var = GetActorLocation();
		var.Z = -1000;
		SetActorLocation(var);
		_StartRespawnTimer = true;
		_RespawnTimer = 5.f;
	}
}

void APlayerCharacter::SetTeam(int Team)
{
	_Team = Team;
	Health->SetTeam(Team);
	_Weapon->SetTeam(_Team);
}

int APlayerCharacter::GetPlayerIndex() const
{
	 return _PlayerIndex; 
}

void APlayerCharacter::SetPlayerIndex(int GivenIndex)
{
	 _PlayerIndex = GivenIndex; 
}

void APlayerCharacter::SwitchWeapon(bool forced)
{
	
		TSubclassOf<AWeapon> BP;
		TSubclassOf<AActor> ParticleBP;
		do
		{
			if (!forced)
			{
				_WeaponIndex = EWeaponType((int(_WeaponIndex) + 1) % int(EWeaponType::Count));
				BP = WeaponInventory[_WeaponIndex];
				ParticleBP = WeaponParticles[_WeaponIndex];
			}
			else
			{
				_WeaponIndex = EWeaponType::Revolver;
				BP = WeaponInventory[_WeaponIndex];
				ParticleBP = WeaponParticles[_WeaponIndex];
			}
			
		} while (BP == NULL);



		RemoveCurrentGun();
		GetGun(BP, ParticleBP);

		IsSwitchingWeapon = true;

	/*if (!_Frozen)
	{
		if (Weapon != nullptr)
		{
			Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Weapon->Destroy();
			Weapon = nullptr;
			IsSwitchingWeapon = true;
			OldWeaponIndex = int(EWeaponIndex::Revolver);
			NewWeaponIndex = int(EWeaponIndex::Golfstick);
			if (MeleeBP != NULL)
			{
				InitMelee();
			}
		}
		else if (Melee != nullptr)
		{
			Melee->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
			Melee->Destroy();
			Melee = nullptr;
			IsSwitchingWeapon = true;
			OldWeaponIndex = int(EWeaponIndex::Golfstick);
			NewWeaponIndex = int(EWeaponIndex::Revolver);
			if (WeaponBP != NULL)
			{
				InitGun();
			}
		}
	}*/
}

template <bool forced>
void APlayerCharacter::SwitchWeapon()
{
	SwitchWeapon();
}


