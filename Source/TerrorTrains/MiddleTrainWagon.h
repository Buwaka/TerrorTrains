// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DropManager.h"
#include "MiddleTrainWagon.generated.h"

UCLASS()
class TERRORTRAINS_API AMiddleTrainWagon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiddleTrainWagon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void Destroyed() override;

	UFUNCTION(BlueprintCallable)
		void SpawnRandomDrops(int amount);
	UFUNCTION(BlueprintCallable)
		void DeleteDrops();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float _Offset = 2.f;
	
private:
	UPROPERTY()
		float _Angle;
	UPROPERTY()
		FRotator _TrainRotation;
	UPROPERTY()
		TArray<ABaseDrop*> _Drops;


};
