// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "MoneyComponent.h"
#include "TrainWalls.generated.h"

/**
 * 
 */
class USFXComponent;
UCLASS()
class TERRORTRAINS_API ATrainWalls : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ATrainWalls();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		UHealthComponent* pHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KillMoney")
		UMoneyComponent* pKillMoney;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
		int Team;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float KllingMoney{25.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USFXComponent* HitSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USFXComponent* DestroySFX;
	//range [0,1]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		float Volume = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		TSubclassOf<AActor> DestroyParticleBP;

private:
	UPROPERTY()
		TArray<UStaticMeshComponent*> pMeshes;
	UPROPERTY()
		float m_HealthPercentage{};
	UPROPERTY()
		float m_HealthChange{};

};
