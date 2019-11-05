// Fill out your copyright notice in the Description page of Project Settings.

#include "SoundManager.h"

#include "Engine/Classes/Kismet/GameplayStatics.h"


// Sets default values
USoundManager::USoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

void USoundManager::Play(USoundWave* sound)
{
	if (_pSounds.Find(sound) == nullptr || sound == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("sound does not exist in map"));
		return;
	}
	if (IsValid(_pSounds[sound]))
	{
		_pSounds[sound]->Play();
	}
	
}

void USoundManager::PlaySimple(UObject* context, USoundWave* sound)
{
	UGameplayStatics::PlaySound2D(context, sound);
}

void USoundManager::PlayAtLocation(UObject* context, USoundWave* sound, FVector location, bool loop)
{
	sound->bLooping = loop;
	UGameplayStatics::PlaySoundAtLocation(context, sound, location);
}

void USoundManager::Pause(USoundWave* sound)
{
	_pSounds[sound]->SetPaused(true);
}

void USoundManager::SetLooping(USoundWave* sound)
{
	sound->bLooping = true;
}

void USoundManager::AddSound(UObject* context, USoundWave* sound, float volume)
{
	if (sound == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("Sound is Null"));
		return;
	}
	auto audiocomp = UGameplayStatics::CreateSound2D(context, sound);
	audiocomp->bAutoDestroy = false;
	audiocomp->VolumeMultiplier = volume;
	if (_pSounds.Find(sound) == nullptr)
	{
		_pSounds.Add(sound, audiocomp);
	}
	else
	{
		_pSounds[sound] = audiocomp;
	}
}

void USoundManager::StopAll()
{
	for (auto& sound : _pSounds)
	{
		if (sound.Value != NULL)
		{
			sound.Value->Stop();
		}
	}
}



