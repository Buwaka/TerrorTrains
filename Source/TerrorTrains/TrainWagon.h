// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include "Door.h"
#include "MoneyVault.h"
#include "TrainWagon.generated.h"

/**
 * 
 */
UCLASS()
class TERRORTRAINS_API ATrainWagon : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ATrainWagon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void TranslateNextTrains(FVector newLocation);
	UFUNCTION(BlueprintCallable)
		void SetTeam(int team);
	UFUNCTION(BlueprintCallable)
		void SetCurrentLocation();
	UFUNCTION(BlueprintCallable)
		int GetTeam();
	UFUNCTION(BlueprintCallable)
		void StopTrain(bool stop = false);
	UFUNCTION(BlueprintCallable)
		void OnTrainHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION(BlueprintCallable)
		bool BuyDoor(APlayerCharacter* player);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		UHealthComponent* pHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
		ATrainWagon* pNextTrain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Team)
		ATrainWagon* pPrevTrain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
		ADoor* pDoor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Train)
		float MoveDuration = 2.0f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float _Offset = 2.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Walls)
		TArray<TSubclassOf<class ATrainWalls>> WallLeftBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Walls)
		TArray < TSubclassOf<class ATrainWalls>> WallRightBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
		TSubclassOf<class ADoor> DoorBP;
	UPROPERTY()
		AMoneyVault* _pMoneyVault;
protected:
	UPROPERTY()
		float _Angle;
	UPROPERTY()
		FRotator _TrainRotation;
	UPROPERTY()
		bool _IsMoving;
	UPROPERTY()
		FVector _CurrentPos;
	UPROPERTY()
		FVector _LastPos;
	UPROPERTY()
		float _Timer;
	UPROPERTY()
		bool _FirstTime;
	UPROPERTY()
		bool _Stop;
	UPROPERTY()
		TArray < ATrainWalls*> _pWallLeft;
	UPROPERTY()
		TArray < ATrainWalls*> _pWallRight;
	UPROPERTY()
		int _Team;
};
