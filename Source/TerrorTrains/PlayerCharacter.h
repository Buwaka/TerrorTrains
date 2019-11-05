// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Weapon.h"
#include "MeleeWeapon.h"
#include "Animation/AnimBlueprint.h"
#include "GameFramework/Character.h"
#include "GameFramework/ForceFeedbackEffect.h"
#include "PlayerCharacter.generated.h"




class USFXComponent;
class UMoneyComponent;
class UHealthComponent;
class UAmmoComponent;
UCLASS(Blueprintable)
class TERRORTRAINS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//VARIABLES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Health")
		UHealthComponent* Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Money")
		UMoneyComponent* Money;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Ammo")
		UAmmoComponent* Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|SFX")
		USFXComponent* DamagedSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon")
		TMap<EWeaponType, TSubclassOf<class AWeapon>> WeaponInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player|Weapon|Partcile")
		TMap<EWeaponType, TSubclassOf<class AActor>> WeaponParticles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
		int OldWeaponIndex{1};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player|Weapon")
		int NewWeaponIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float SavedSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float KllingMoney{50.f};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DamageRumble")
		UForceFeedbackEffect* HitRumble;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "IsRespawn")
		bool _Respawn{ false };

	//Animation variables
	//melee
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsHitting{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsSwitchingWeapon{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsWalking{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsAiming{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsShooting{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsStationary{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float Direction{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float IndicatorScale{0.3f};


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool ShowIndicator{ true };

	//FUNCTIONS

	//Movement
	UFUNCTION(BlueprintCallable)
		void CheckMove();

	UFUNCTION(BlueprintCallable)
		void MoveForward(float Value);

	UFUNCTION(BlueprintCallable)
		void MoveRight(float Value);

	UFUNCTION(BlueprintCallable)
		void RotateForward(float Value);

	UFUNCTION(BlueprintCallable)
		void RotateRight(float Value);

	UFUNCTION(BlueprintCallable)
		void Interact();

	UFUNCTION(BlueprintCallable)
		bool WantsToInteract();

	UFUNCTION(BlueprintCallable)
		void SetAiming();

	//Weapons
	UFUNCTION(BlueprintCallable)
	void GetGun(TSubclassOf<class AWeapon> BP, TSubclassOf<class AActor> switchAnimationBP = NULL);

	UFUNCTION(BlueprintCallable)
	void RemoveCurrentGun();

	UFUNCTION(BlueprintCallable)
	void InitGuns();



	UFUNCTION(BlueprintCallable)
	AWeapon* GetWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequence* PreviouwWeaponSwitchAnimation;


	UFUNCTION(BlueprintCallable)
		void FireGun();

	UFUNCTION(BlueprintCallable)
		void SwitchWeapon(bool forced = false);

	template<bool forced>
	UFUNCTION(BlueprintCallable)
		void SwitchWeapon();


	//Update

	UFUNCTION(BlueprintCallable)
		void CheckHealth();


	//Respawning
	UFUNCTION(BlueprintCallable)
		bool IsRespawn() const { return _Respawn; };

	UFUNCTION(BlueprintCallable)
		bool IsDestroy() const { return _Destroy; };

	UFUNCTION(BlueprintCallable)
		void ResetRespawn()
	{
		_Respawn = false;
	};

	UFUNCTION(BlueprintCallable)
		void ActivateRespawn(){_Respawn = true;};


	//Team
	UFUNCTION(BlueprintCallable)
		void SetTeam(int Team);

	UFUNCTION(BlueprintCallable)
		int GetTeam() const { return _Team; };

	UFUNCTION(BlueprintCallable)
		int GetPlayerIndex() const;

	UFUNCTION()
		void SetPlayerIndex(int GivenIndex);

private:
	//Movement variables
	UPROPERTY()
	float _ValueX{};
	UPROPERTY()
	float _ValueY{};
	UPROPERTY()
	float _MoveY{};
	UPROPERTY()
	float _MoveX{};

	//State bools
	UPROPERTY()
	bool _CanInteract{};

	UPROPERTY()
	bool _Destroy{ false };
	UPROPERTY()
	bool _HasSetHealth{false};
	UPROPERTY()
	bool _StartRespawnTimer{false};
	//Weapons
	UPROPERTY()
	AWeapon* _Weapon;
	
	UPROPERTY()
	EWeaponType _WeaponIndex{EWeaponType::Revolver};

	//Team
	UPROPERTY()
	int _Team{};

	UPROPERTY()
	int _PlayerIndex{};

	UPROPERTY()
	float _RespawnTimer{};

};
