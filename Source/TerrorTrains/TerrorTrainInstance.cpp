// Fill out your copyright notice in the Description page of Project Settings.

#include "TerrorTrainInstance.h"

UTerrorTrainInstance::UTerrorTrainInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SoundManager = NewObject<USoundManager>();
	//DropManager = NewObject<UDropManager>();
}

void UTerrorTrainInstance::Reset()
{
	DropManager->DeleteDrops();
	SoundManager->StopAll();
}

USoundManager * UTerrorTrainInstance::GetSoundManager()
{
	return SoundManager;
}

ADropManager * UTerrorTrainInstance::GetDropManager()
{
	return DropManager;
}

ACharacterManager * UTerrorTrainInstance::GetCharacterManager()
{
	return CharacterManager;
}

AOverworldManager * UTerrorTrainInstance::GetOverworldManager()
{
	return OverworldManager;
}

AStationManager * UTerrorTrainInstance::GetStationManager()
{
	return StationManager;
}

ATrainManager * UTerrorTrainInstance::GetTrainManager()
{
	return TrainManager;
}

void UTerrorTrainInstance::SetTrainManager(ATrainManager * manager)
{
	TrainManager = manager;
}

void UTerrorTrainInstance::SetStationManager(AStationManager * manager)
{
	StationManager = manager;
}

void UTerrorTrainInstance::SetOverworldManager(AOverworldManager * manager)
{
	OverworldManager = manager;
}

void UTerrorTrainInstance::SetCharacterManager(ACharacterManager * manager)
{
	CharacterManager = manager;
}
void UTerrorTrainInstance::SetDropManager(ADropManager * manager)
{
	DropManager = manager;
}
