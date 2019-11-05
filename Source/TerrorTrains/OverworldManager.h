// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterManager.h"
#include "TrainManager.h"
#include "StationManager.h"
#include "RailsTrackGenaration.h"
#include "SFXComponent.h"
#include "OverworldManager.generated.h"

class UUserWidget;
UCLASS()
class TERRORTRAINS_API AOverworldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOverworldManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BluePrintCallable)
		void StartGame();
	UFUNCTION(BluePrintCallable)
		void PauseGame();
	UFUNCTION(BluePrintCallable)
		void ResetGame();
	UFUNCTION()
		void DoWin(int team);
	UFUNCTION(BluePrintCallable)
		int GetNumberOfWagons(int team);
	UFUNCTION(BluePrintCallable)
		float GetMoney(int team);
	UFUNCTION(BluePrintCallable)
		bool CheckWinOfTeam(int team);
	UFUNCTION(BluePrintCallable)
		float GetGameTime();
	UFUNCTION(BlueprintCallable)
		int GetTeamDeaths(int team);
	UFUNCTION(BlueprintCallable)
		int GetRemainingDoors();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Train")
		int NumOfWagons = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiddleTrain")
		TSubclassOf<AMiddleTrain> BPMiddleTrain;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundGeneration")
		TSubclassOf<ARailsTrackGenaration> BPTrack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundGeneration")
		int NrOfStartObjects{5};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundGeneration")
		FVector GroundLoc {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundGeneration")
		FRotator GroundRot {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GroundGeneration")
		FVector GroundSpawnOffset {0.f,50.f,0.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Drops")
		TSubclassOf<ADropManager> BPDrop;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiddleTrain")
		FVector MiddleTrainLoc {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiddleTrain")
		FRotator MiddleTrainRot {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wEOGMenu;
	UPROPERTY()
	UUserWidget* MyEOGMenu;
	UPROPERTY(BlueprintReadOnly)
	FString WinQuote;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BGM")
	USFXComponent* BGM = CreateDefaultSubobject<USFXComponent>("Audio");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
	USFXComponent* eogSFX = CreateDefaultSubobject<USFXComponent>("SFX");

private:
	UPROPERTY()
	ACharacterManager* _pCharManager;
	UPROPERTY()
	ATrainManager * _pTrainManager;
	UPROPERTY()
	AStationManager* _pStationManager;
	UPROPERTY()
	AMiddleTrain* _pMiddleTrain;
	UPROPERTY()
	ARailsTrackGenaration* _pTrackGen;
	UPROPERTY()
	float _GameTime{};
	UPROPERTY()
	bool _FirstTime = true;
	UPROPERTY()
		bool _FirstTimeRiding = false;
	UPROPERTY()
		bool _Team1Wins = false;
	UPROPERTY()
		bool _Team2Wins = false;
	UFUNCTION()
	void CheckMiddleTrain();
	UFUNCTION()
	void CheckStation();
	UFUNCTION()
	void CheckBuyDoor();
	
};
