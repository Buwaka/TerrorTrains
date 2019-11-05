// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "TerrorTrains/MoneyComponent.h"
#include "BaseDrop.generated.h"

class USFXComponent;
UCLASS()
class TERRORTRAINS_API ABaseDrop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseDrop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void Activate(AActor* OtherActor, const FHitResult& Hit) {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseDrop")
		UStaticMeshComponent *DropMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseDrop")
		UMoneyComponent *Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseDrop|Sound")
		USFXComponent *PickedUpSFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseDrop|Particle")
		TSubclassOf<AActor> ParticleBP;
};
