// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"


#include "CharacterManager.h"
#include "DropManager.h"
#include "OverworldManager.h"
#include "SoundManager.h"
#include "StationManager.h"
#include "TrainManager.h"

#include "TerrorTrainInstance.generated.h"

#define TerrorTrainInstance Cast<UTerrorTrainInstance>(GetGameInstance())
#define TerrorTrainInstanceFromComponent	Cast<UTerrorTrainInstance>(GetOwner()->GetWorld()->GetGameInstance())

UCLASS()
class TERRORTRAINS_API UTerrorTrainInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTerrorTrainInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void Reset();

	UFUNCTION()
	USoundManager* GetSoundManager();
	UFUNCTION()
	ADropManager* GetDropManager();
	UFUNCTION()
	ACharacterManager* GetCharacterManager();

	UFUNCTION()
	AOverworldManager* GetOverworldManager();

	UFUNCTION()
	AStationManager* GetStationManager();

	UFUNCTION()
	ATrainManager* GetTrainManager();

protected:
	friend ATrainManager;
	friend AStationManager;
	friend AOverworldManager;
	friend ACharacterManager;
	friend ADropManager;

	UFUNCTION()
	void SetTrainManager(ATrainManager* manager);
	UFUNCTION()
	void SetStationManager(AStationManager* manager);
	UFUNCTION()
	void SetOverworldManager(AOverworldManager* manager);
	UFUNCTION()
	void SetCharacterManager(ACharacterManager* manager);
	UFUNCTION()
	void SetDropManager(ADropManager * manager);

private:
	UPROPERTY()
	USoundManager* SoundManager;
	UPROPERTY()
	ADropManager* DropManager;
	UPROPERTY()
	ACharacterManager* CharacterManager;
	UPROPERTY()
	AOverworldManager* OverworldManager;
	UPROPERTY()
	AStationManager* StationManager;
	UPROPERTY()
	ATrainManager* TrainManager;
};
