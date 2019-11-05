// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h" 
#include "GameFramework/Actor.h"
#include "Audio.h"
#include "Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundWave.h"
#include "SoundManager.generated.h"


//USTRUCT()
//struct F_Audio
//{
//	GENERATED_BODY()
//		F_Audio() {};
//		F_Audio(USoundWave* sound)
//		{
//			soundWave = sound;
//			audioComp = nullptr;
//		};
//
//	UPROPERTY()
//		USoundWave* soundWave;
//	UPROPERTY()
//		UAudioComponent* audioComp;
//};

UCLASS()
class TERRORTRAINS_API USoundManager : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	USoundManager();

protected:

	
public:	

	UFUNCTION(BlueprintCallable)
		//context is just this pointer from an object that exists in "World"
	void PlayAtLocation(UObject* context, USoundWave* sound, FVector location, bool loop = false);

	UFUNCTION(BlueprintCallable)
	void Play(USoundWave* sound);
	UFUNCTION(BlueprintCallable)
		//doesn't need to be added to the sound map/list, don't do this with loopable sounds as they can't be accessed afterwards
	void PlaySimple(UObject* context, USoundWave* sound);

	UFUNCTION()
	void Pause(USoundWave* sound);
	UFUNCTION()
	void SetLooping(USoundWave* sound);
	UFUNCTION(BlueprintCallable)
	//context is just this pointer from an object that exists in "World"
	void AddSound(UObject* context, USoundWave* sound, float volume = 1.0f);

	UFUNCTION(BlueprintCallable)
	void StopAll();

private:


	UPROPERTY()
	TMap<USoundWave*, UAudioComponent* > _pSounds;
	//UPROPERTY()
	//unsigned int _Counter;
};
