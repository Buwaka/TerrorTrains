// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseDrop.h"
#include "Weapon.h"
#include "WeaponDrop.generated.h"

/**
 * 
 */
UCLASS()
class TERRORTRAINS_API AWeaponDrop : public ABaseDrop
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AWeaponDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Activate(AActor* OtherActor, const FHitResult& Hit) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	AWeapon* WeaponDrop;

private:
	UFUNCTION(BlueprintCallable)
	void Randomize();
};
