// Fill out your copyright notice in the Description page of Project Settings.

#include "SFXComponent.h"
#include "TerrorTrainInstance.h"
#include "SoundManager.h"

// Sets default values for this component's properties
USFXComponent::USFXComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USFXComponent::BeginPlay()
{
	Super::BeginPlay();
	_Volume = Volume;
	TerrorTrainInstanceFromComponent->GetSoundManager()->AddSound(this, SFX, _Volume);
}

void USFXComponent::Play()
{
	TerrorTrainInstanceFromComponent->GetSoundManager()->Play(SFX);
}

void USFXComponent::Pause()
{
	TerrorTrainInstanceFromComponent->GetSoundManager()->Pause(SFX);
}




