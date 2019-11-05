// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SoundManager.h"
#include "Runtime/Engine/Classes/Animation/AnimSequence.h"
#include "Runtime/Engine/Classes/Engine/Texture2D.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "Weapon.generated.h"

class APlayerCharacter;
class USFXComponent;
UENUM()
enum class EWeaponType : uint8
{
	Revolver,
	Submachine,
	TNTGun,
	ScatterGun,
	Melee,
	Count
};

UCLASS(Blueprintable)
class TERRORTRAINS_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//functions
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	virtual bool Shoot(AActor* shootingPlayer);

	UFUNCTION(BlueprintCallable)
	int GetTeam();

	UFUNCTION(BlueprintCallable)
	void SetTeam(int team);

	UFUNCTION(BlueprintCallable)
	float GetDamage();

	UFUNCTION(BlueprintCallable)
	void SetDamage(int damage);

	UFUNCTION(BlueprintCallable)
	float GetRateOfFire();

	UFUNCTION(BlueprintCallable)
	void SetRateOfFire(int ROF);

	UFUNCTION(BlueprintCallable)
	float GetSpread();

	UFUNCTION(BlueprintCallable)
	void SetSpread(int spread);

	UFUNCTION(BlueprintCallable)
	void SetSocket(FName Socket);

	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetIdleAnimation();

	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetHipShootAnimation();

	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetAimShootAnimation();

	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetSwitchAnimation();

	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetAimLoopAnimation();

	UFUNCTION(BlueprintCallable)
	UAnimSequence* GetBeginAnimation();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rumble")
	UForceFeedbackEffect* FireRumble;

	//variables
	//weapon mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	USkeletalMeshComponent* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class ABullet> Bullet;
	
	//Socket where the bullet will spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName BulletSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Audio")
	USoundWave* GunShotSFX;

	//range [0,1]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Audio")
	float Volume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimSequence* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimSequence* AimLoopAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimSequence* AimShootAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimSequence* HipShootAnimation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimSequence* SwitchAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Animation")
	UAnimSequence* BeginAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|UI")
	UTexture2D* WeaponUIPreview;

	//Use damage set in weapon or use damage from the bullet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool OverrideDamage = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DefaultDamage = 50.f;

	//speed in physics force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DefaultSpeed = 3000.f;

	// in bullets per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DefaultRateOfFire = 1.f;

	// in degrees, 4.5 is [-4.5,4.5] around the center direction, 0 is no spread
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float DefaultSpread = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int DefaultBulletsPerShot = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int DefaultCostPerBullet = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool Stationary = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USFXComponent* OutOfAmmoSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		TSubclassOf<class AActor> ParticleBP;




protected:


	UPROPERTY()
	float _Damage;
	UPROPERTY()
	float _Speed;
	UPROPERTY()
	float _RateOfFire;
	UPROPERTY()
	unsigned int _BulletsPerShot;
	UPROPERTY()
	int _CostPerBullet;
	UPROPERTY()
	float _Spread;
	UPROPERTY()
	int _Team;
	UPROPERTY()
	float _Timer;
	UPROPERTY()
	FVector _RelBulletLocation;

	UPROPERTY()
		float _AnimCoolDown{ 0.f };
	UPROPERTY()
		bool _StartAnimCoolDown{};
};
