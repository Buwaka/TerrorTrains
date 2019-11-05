// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TerrorTrains/MoneyComponent.h"
#include "PlayerCharacter.h"
#include "Components/BoxComponent.h"
#include "Door.generated.h"

class USFXComponent;
UCLASS()
class TERRORTRAINS_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	bool BuyDoor(APlayerCharacter* player);

	UFUNCTION()
		bool CanBuyDoor();

	UFUNCTION()
		void DoAfford();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent *pMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Money")
		UMoneyComponent *pCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UBoxComponent* HitBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		USFXComponent*pOpenDoorSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		TSubclassOf<AActor> CanBuyParticleBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Particle")
		TSubclassOf<AActor> BoughtParticleBP;

private:
	UPROPERTY()
		TArray<bool> _CanBuy;
	UPROPERTY()
		bool _Particle{false};
	UPROPERTY()
		AActor* m_Particle {};
};
