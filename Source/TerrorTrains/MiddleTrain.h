// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "SFXComponent.h"
#include "Animation/AnimSequence.h"
#include "MiddleTrain.generated.h"

UCLASS()
class TERRORTRAINS_API AMiddleTrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiddleTrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void CheckIfTrainCanStop(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CheckIfTrainCanBeResetted();
	UFUNCTION()
		void IsStationRiding(bool canI = false);
	UFUNCTION()
		bool GetIsRide();
	UFUNCTION()
		void AddWagon(int num = 1);
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	//	UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* pAnimationMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UBoxComponent* BackHitBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = TrainSpeed)
		FVector TrainSpeed = FVector(0, 400.f, 0);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wagons)
		TSubclassOf<class AMiddleTrainWagon> WagonBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USFXComponent* trainLeavingSFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimSequence* pAnim {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DeleteTime)
		float _DeleteTime{ 20.f };
private:
	UPROPERTY()
		TArray<AMiddleTrainWagon*> _pWagons;
	UPROPERTY()
		bool _FirstTimeUpdate{ true };
	UPROPERTY()
	bool _IsFirstTime{};
	UPROPERTY()
	bool _IsOnPosition{};
	UPROPERTY()
	bool _CanRide{};
	UPROPERTY()
	bool _IsRiding{};
	UPROPERTY()
		bool _SFXHasPlayed{};
	UPROPERTY()
	float _Timer = 20.0f;
	UPROPERTY()
	float _WaitTimer = 0;
	UPROPERTY()
	FVector _BeginPos{};
	UPROPERTY()
	FVector _MeshSize{};
};
