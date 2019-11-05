// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiddleTrain.h"
#include "DropManager.h"
#include "Station.generated.h"

class SpawnPoint;

UCLASS()
class TERRORTRAINS_API AStation : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStation();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		void SpawnRandomDrops(int amount);
	UFUNCTION(BlueprintCallable)
		void DeleteDrops();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent *pStationMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Offset")
		FVector Offset{};
private:

};
