// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TerrorTrains/Interactable.h"
#include "TerrorTrains/PlayerCharacter.h"
#include "Stationary.generated.h"

UCLASS()
class TERRORTRAINS_API AStationary : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStationary();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void Mount(UObject* player);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		TSubclassOf<class AWeapon> WeaponBP;


private:
	//UPROPERTY()
	//UInteractable* _Interable;
	UPROPERTY()
	AWeapon* _Weapon;
};
