// Fill out your copyright notice in the Description page of Project Settings.

#include "RailsTrackGenaration.h"
#include "Engine/World.h"

// Sets default values
ARailsTrackGenaration::ARailsTrackGenaration()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARailsTrackGenaration::BeginPlay()
{
	Super::BeginPlay();
	_OGSpeed = Speed;	
}

// Called every frame
void ARailsTrackGenaration::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_TranslateRails)
	{
		for (auto rail : _pRails)
		{
			if (IsValid(rail))
			{
				rail->SetActorLocation(rail->GetActorLocation() - (Speed * DeltaTime));
			}
		}
		if (_pSplitRails.Num() != 0.f)
		{
			for (auto rail : _pSplitRails)
			{
				if (IsValid(rail))
				{
					rail->SetActorLocation(rail->GetActorLocation() - (Speed * DeltaTime));
				}
			}
		}
	}
	if (_MakeRails)
	{		
		const FVector checkLoc = _pRails[0]->GetActorLocation() + Offset;
		if (checkLoc.Y <= _OGSpawnPoint.Y)
		{
			if (DontDelete)
			{
				DoNotDelete();
			}
			else
			{
				DoDelete();
			}

		}
	}	
}

void ARailsTrackGenaration::MakeRails(FVector StartPos, FRotator Rotation, int nrOfRails, bool connectingSides)
{
	auto type = GiveRandomValue();
	if (BPRailMap[type] != NULL)
	{
		auto firstRail = _pRails.Add(GetWorld()->SpawnActor<AActor>(BPRailMap[type], StartPos, Rotation, FActorSpawnParameters()));
		FVector bounds{};
		FVector org{};
		_pRails[firstRail]->GetActorBounds(true, org, bounds);
		_OGSpawnPoint = StartPos;
		_OGSpawnPoint.Y += (bounds.Y*2.f);		
		_pRails[firstRail]->SetLifeSpan(60.f);
		_SpawnPoint = StartPos;
		_SpawnPoint.Y += (bounds.Y*2.f) + Offset.Y;
		_SpawnRot = Rotation;
		_pRails[firstRail]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

		FVector middle = StartPos;
		for (int i = 1; i < nrOfRails; i++)
		{
			type = GiveRandomValue();
			//middle track

			middle.Y -= (bounds.Y*2.f);
			firstRail = _pRails.Add(GetWorld()->SpawnActor<AActor>(BPRailMap[type], middle, Rotation));
			_pRails[firstRail]->SetLifeSpan(_Lifetime);
			_pRails[firstRail]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
	}
}

void ARailsTrackGenaration::MakeRailsAtEnd(int nrOfRails)
{
	auto type = GiveRandomValue();
	if (BPRailMap[type] != NULL && _pRails.Num() != 0)
	{
		auto rails = _pRails.Last();
		FVector bounds{};
		FVector org{};
		rails->GetActorBounds(true, org, bounds);
		FVector loc = rails->GetActorLocation();
		for (int i = 0; i < nrOfRails; i++)
		{
			loc.Y -= ((bounds.Y*2.f) + Offset.Y);
			auto firstRail = _pRails.Add(GetWorld()->SpawnActor<AActor>(BPRailMap[type], loc, rails->GetActorRotation(), FActorSpawnParameters()));
			_pRails[firstRail]->SetLifeSpan(_Lifetime);
			_pRails[firstRail]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		}
		
	}
}

void ARailsTrackGenaration::KeepMakingRails(bool make)
{
	_MakeRails = make;
}

void ARailsTrackGenaration::StopRailsMoving(bool stop)
{
	_TranslateRails = !stop;
	if (stop)
	{
		SetLifeTime(0.f);
	}
	else
	{
		SetLifeTime(40.f);
	}
}

void ARailsTrackGenaration::SetLifeTime(float lifetime)
{
	_Lifetime = lifetime;
	for (auto rail : _pRails)
	{
		if (IsValid(rail))
		{
			rail->SetLifeSpan(lifetime);
		}
	}
}

void ARailsTrackGenaration::DoSplit(bool doBack)
{
	if (BPSplitRail != NULL && _pRails.Num() != 0)
	{
		if (!doBack)
		{
			auto rails = _pRails[0];
			FVector bounds{};
			FVector org{};
			rails->GetActorBounds(true, org, bounds);
			FVector loc = rails->GetActorLocation();
			loc.Y += (bounds.Y*2.f);
			auto firstRail = _pRails.Insert(GetWorld()->SpawnActor<AActor>(BPSplitRail, loc, rails->GetActorRotation(), FActorSpawnParameters()),0);
			_pRails[firstRail]->SetLifeSpan(_Lifetime);
			_pRails[firstRail]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			_pRails[firstRail]->GetActorBounds(true, org, bounds);
			_SplitsSpawnPoint = _SpawnPoint;
			_SplitsSpawnPoint.X += (bounds.X *0.85f);
			_IsSplitsed = true;
			_Wait = true;
			
		}
		else
		{
			auto rails = _pSplitRails[0];
			FVector bounds{};
			FVector org{};
			rails->GetActorBounds(true, org, bounds);
			FVector loc = rails->GetActorLocation();
			loc.Y += (bounds.Y*2.f);
			auto firstRail = _pSplitRails.Insert(GetWorld()->SpawnActor<AActor>(BPSplitRail, loc, rails->GetActorRotation(), FActorSpawnParameters()), 0);
			_pSplitRails[firstRail]->SetLifeSpan(_Lifetime);
			_pSplitRails[firstRail]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			_IsSplitsed = false;
		}
	}
}

void ARailsTrackGenaration::SlowDown()
{
	Speed = FMath::Lerp(Speed, { 0,0,0 }, TimeOfTranslate);
}

void ARailsTrackGenaration::BackToNormalSpeed()
{
	Speed = FMath::Lerp(Speed, _OGSpeed, 0.3f);
}

FString ARailsTrackGenaration::GiveRandomValue()
{
	TArray<FString> keyArray = TArray<FString>();
	BPRailMap.GenerateKeyArray(keyArray);

	auto rand = FMath::RandRange(0, keyArray.Num() - 1);
	return keyArray[rand];
}

void ARailsTrackGenaration::DoDelete()
{
	auto type = GiveRandomValue();
	FVector bounds{};
	FVector org{};
	_pRails[0]->GetActorBounds(true, org, bounds);
	auto loc = _pRails[0]->GetActorLocation();
	loc.Y += (bounds.Y*2.f);

	_pRails.Insert(GetWorld()->SpawnActor<AActor>(BPRailMap[type], loc, _SpawnRot, FActorSpawnParameters()), 0);
	_pRails[0]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	_pRails[0]->SetLifeSpan(_Lifetime);
	if (_IsSplitsed)
	{
		_SplitsSpawnPoint.Y = loc.Y;
		_pSplitRails.Insert(GetWorld()->SpawnActor<AActor>(BPRailMap[type], _SplitsSpawnPoint, _SpawnRot, FActorSpawnParameters()), 0);
		_pSplitRails[0]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		_pSplitRails[0]->SetLifeSpan(_Lifetime);
	}
}

void ARailsTrackGenaration::DoNotDelete()
{
	SetLifeTime(0.f);
	auto type = GiveRandomValue();
	FVector bounds{};
	FVector org{};
	_pRails[0]->GetActorBounds(true, org, bounds);
	auto loc = _pRails[0]->GetActorLocation();
	loc.Y += (bounds.Y*2.f);

	_pRails.Last()->SetActorLocation(loc);
	for (int i = 1; i < _pRails.Num(); i++)
	{
		_pRails.Swap(0, i);
	}
	/*if (_IsSplitsed)
	{
		_SplitsSpawnPoint.Y = loc.Y;
		_pSplitRails.Insert(GetWorld()->SpawnActor<AActor>(BPRailMap[type], _SplitsSpawnPoint, _SpawnRot, FActorSpawnParameters()), 0);
		_pSplitRails[0]->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		_pSplitRails[0]->SetLifeSpan(_Lifetime);
	}*/
}

