// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AmmoComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRORTRAINS_API UAmmoComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAmmoComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool LoadAmmo(int amount);
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	int GetAmmo();
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void AddAmmo(int amount);
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void RemoveAmmo(int amount);
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void SetAmmo(int amount);
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int DefaultAmmoCount = INT32_MAX - 1;


private:
	UPROPERTY()
	unsigned int _Ammo;
};
