// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RailsTrackGenaration.generated.h"

UCLASS()
class TERRORTRAINS_API ARailsTrackGenaration : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARailsTrackGenaration();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
		void MakeRails(FVector StartPos, FRotator Rotation, int nrOfRails, bool connectingSides = false);
	UFUNCTION()
		void MakeRailsAtEnd(int nrOfRails);
	UFUNCTION()
		void KeepMakingRails(bool make = true);
	UFUNCTION()
		void StopRailsMoving(bool stop = true);
	UFUNCTION()
		void SetLifeTime(float lifetime);
	UFUNCTION()
		void DoSplit(bool doBack = false);
	UFUNCTION()
		void SlowDown();
	UFUNCTION()
		void BackToNormalSpeed();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		TMap<FString, TSubclassOf<AActor>> BPRailMap;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	TSubclassOf<AActor> BPSplitRail;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
		FVector Speed {};
	UPROPERTY()
		FVector Offset {0.f,0.f,0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
		int MaxTrackSize{ 20 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sort")
		bool DontDelete{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SlowDown")
		float TimeOfTranslate{ 0.01f };
private:
	UFUNCTION()
		FString GiveRandomValue();
	UFUNCTION()
		void DoDelete();
	UFUNCTION()
		void DoNotDelete();
	UPROPERTY()
	TArray<AActor*> _pRails;
	UPROPERTY()
		TArray<AActor*> _pSplitRails;
	UPROPERTY()
	FVector _SpawnPoint{};
	UPROPERTY()
		FVector _OGSpawnPoint {};
	UPROPERTY()
	FRotator _SpawnRot{};
	UPROPERTY()
		FVector _SplitsSpawnPoint {};
	UPROPERTY()
	bool _MakeRails{};
	UPROPERTY()
	bool _TranslateRails{};
	UPROPERTY()
	float _Lifetime{};
	UPROPERTY()
		bool _IsSplitsed{};
	UPROPERTY()
		bool _Wait{};
	UPROPERTY()
		FVector _OGSpeed {};
};
