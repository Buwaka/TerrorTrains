// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"


// enkel shoot animatie, rate of fire, damage en mesh moet gezet worden

UCLASS()
class TERRORTRAINS_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeleeWeapon();

protected:
	// Called when the game starts or when spawned
	void BeginPlay();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Shoot(AActor* shootingPlayer) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Static Mesh")
	UStaticMeshComponent* Mesh;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SetWeaponMeshOnIndex(int index);

private:
	UPROPERTY()
	bool _Swing;

	UPROPERTY()
	TArray<UStaticMeshComponent*> pMeshes;
};
