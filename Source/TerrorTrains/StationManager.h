// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Station.h"
#include "StationManager.generated.h"

class USFXComponent;
UCLASS()
class TERRORTRAINS_API AStationManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStationManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void IsMiddleTrainRiding(bool canIt = false);
	UFUNCTION()
		bool GetIsRiding();
	UFUNCTION(BluePrintCallable)
		bool GetIsArrived();
	UFUNCTION()
		void HasArrived(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void HasLeft();
	UFUNCTION(BlueprintCallable)
		void ChangeToSplitsScreen(AActor* Context, bool bDisable);
	UFUNCTION()
		void SetScaleMultiplier(FVector scale);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UBoxComponent* HitBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StationSpeed)
		FVector StationSpeed = FVector(0, -500.f, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Drops)
		TSubclassOf<AStation> StationBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaionLoc)
		FVector StationLeftLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaionLoc)
		FVector StationRightLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaionLoc)
		FRotator StationLeftRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StaionLoc)
		FRotator StationRightRotation;
	UPROPERTY()
		bool IsLeaving = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeleteTime)
		float _DeleteTime{ 20.f };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		USFXComponent* LeavingSFX;
private:
	UPROPERTY()
		bool _FirstTime{ true };
	UPROPERTY()
		bool _SpawnDrops{ true };
	UPROPERTY()
		bool _FirstTimeUpdate{ true };
	UPROPERTY()
		bool _CanRide{ true };
	UPROPERTY()
		bool _IsRiding{ false };
	UPROPERTY()
		bool _IsArrived;
	UPROPERTY()
		bool _SFXHasPlayed;
	UPROPERTY()
		AStation* _pStationLeft;
	UPROPERTY()
		AStation* _pStationRight;
	UPROPERTY()
		FVector _StartingPos;
	UPROPERTY()
		FVector _ScaleMulti{1,1,1};
	UPROPERTY()
		FVector _StationLeftScale;
	UPROPERTY()
		FVector _StationRightScale;
	UPROPERTY()
		float _StopTime;
	UPROPERTY()
		float _WaitTime;
	
};
