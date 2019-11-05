// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Train.h"
#include "PlayerCharacter.h"
#include "TrainManager.generated.h"

UCLASS()
class TERRORTRAINS_API ATrainManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrainManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable, Category = Team)
		ATrain* GetTeam1();
	UFUNCTION(BlueprintCallable, Category = Team)
		ATrain* GetTeam2();
	UFUNCTION(BlueprintCallable, Category = Team)
		void AddTeam(FVector loc, FRotator rotation, FVector spaceInBetween);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		void AddWagon(int team);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		void SetMaxWagon(int maxwagon);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		void SetStartingWagon(int startingwagon);
	UFUNCTION(BlueprintCallable, Category = Health)
		void SetHealth(int team, float health);
	UFUNCTION(BlueprintCallable, Category = Rails)
		void SetTrainStop(bool Stop = false);
	UFUNCTION(BlueprintCallable, Category = Rails)
		void DoRailsBreaking();
	UFUNCTION(BlueprintCallable, Category = Rails)
		void DoRailsNotBreaking();
	UFUNCTION(BlueprintCallable, Category = Rails)
		void SetTrainBreaking(bool Stop = true);
	UFUNCTION(BlueprintCallable, Category = Health)
		void MoveOut(bool start, bool moveOut);
	UFUNCTION(BlueprintCallable, Category = Health)
		bool GetDoneMoveOut();
	UFUNCTION(BlueprintCallable, Category = Health)
		bool TryBuyDoor(int team, APlayerCharacter* player);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Team1")
		TSubclassOf<class ATrain> TrainBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Team2")
		TSubclassOf<class ATrain> TrainBP2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Team1")
		FVector LocationTrain1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Team2")
		FVector LocationTrain2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Team1")
		FRotator RotationTrain1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|Team2")
		FRotator RotationTrain2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train|MoveOut")
		FVector MoveOutSpeed;

	UFUNCTION(BlueprintCallable, Category = Wagon)
		int GetDoorCount(int team);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		int GetOpenDoorCount(int team);

private:
	ATrain* _pTeam1;
	ATrain* _pTeam2;
};
