// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRORTRAINS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// don't forget to set the team, Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//functions
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Initialize(int team = 0,float health = -1.f, float invincibilityTime = -0.1f);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float health);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void RemoveHealth(float health, int team = 1, bool TriggerInvincible = true);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetOverHealth();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ResetHealth(float health = -1.f);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetInvincible(float duration);

	UFUNCTION(BlueprintCallable, Category = "Health")
	int GetTeam();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetTeam(int team);

	//variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultInvincibilityTime = -0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	int DefaultTeam = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Particle")
		TSubclassOf<AActor> ParticleBP;

private:
	UPROPERTY()
	float _Health;
	UPROPERTY()
	float _Timer;
	UPROPERTY()
	int _Team;
	UPROPERTY()
	float _InvincibilityTime;
	UPROPERTY()
	bool _Hit;
};
