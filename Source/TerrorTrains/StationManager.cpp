// Fill out your copyright notice in the Description page of Project Settings.

#include "StationManager.h"
#include "UObjectIterator.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "Train.h"
#include "TrainWagon.h"
#include "SFXComponent.h"
#include "TerrorTrainInstance.h"

// Sets default values
AStationManager::AStationManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	LeavingSFX = CreateDefaultSubobject<USFXComponent>("SFX");
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AStationManager::HasArrived);
}

// Called when the game starts or when spawned
void AStationManager::BeginPlay()
{
	Super::BeginPlay();

	TerrorTrainInstance->SetStationManager(this);

	_WaitTime = FMath::FRandRange(10.f, 40.f);
	_StopTime = FMath::FRandRange(30.f, 60.f);
	_StartingPos = GetActorLocation();
	_FirstTime = true;
	if (StationBP != NULL)
	{
		FActorSpawnParameters SpawnParams;
		_pStationLeft = GetWorld()->SpawnActor<AStation>(StationBP, StationLeftLocation, StationLeftRotation, SpawnParams);
		_pStationLeft->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		

		_pStationRight = GetWorld()->SpawnActor<AStation>(StationBP, StationRightLocation, StationRightRotation, SpawnParams);
		_pStationRight->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
		

		_StationLeftScale = _pStationLeft->GetActorRelativeScale3D();
		_StationRightScale = _pStationRight->GetActorRelativeScale3D();
	}
}

// Called every frame
void AStationManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_SpawnDrops)
	{
		_pStationLeft->SpawnRandomDrops(20);
		_pStationRight->SpawnRandomDrops(20);
		_SpawnDrops = false;
	}
	if (_CanRide)
	{
		if (_WaitTime > 0)
		{
			_WaitTime -= DeltaTime;
			if (_WaitTime <= 0)
			{
				_IsRiding = true;
			}
			return;
		}
		if (_IsRiding)
		{
			_IsRiding = true;
			FVector temp = { this->GetActorLocation() };
			temp += StationSpeed * DeltaTime;
			this->SetActorLocation(temp);
			
			if (IsLeaving)
			{
				_DeleteTime -= DeltaTime;
				HasLeft();
			}
			
			return;
		}
		if (_StopTime <= 5.f && !_SFXHasPlayed)
		{
			LeavingSFX->Play();
			_SFXHasPlayed = true;
		}
		if (_StopTime <= 0)
		{
			_FirstTime = false;
			_IsRiding = true;
			IsLeaving = true;
			_SFXHasPlayed = false;
			_IsArrived = false;
			return;
		}
		_StopTime -= DeltaTime;
	}
}

void AStationManager::IsMiddleTrainRiding(bool canIt)
{
	_CanRide = !canIt;
}

bool AStationManager::GetIsRiding()
{
	return _IsRiding;
}

bool AStationManager::GetIsArrived()
{
	return _IsArrived;
}

void AStationManager::HasArrived(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_FirstTime)
	{
		if (OtherActor->IsA(ATrain::StaticClass()) || OtherActor->IsA(ATrainWagon::StaticClass()))
		{
			_StopTime = FMath::FRandRange(40.f, 60.f);
			_IsArrived = true;
			_IsRiding = false;
		}
	}
}

void AStationManager::HasLeft()
{
	if (!_FirstTime)
	{
		if (_DeleteTime <= 0)
		{
			_pStationRight->Destroy();
			_pStationLeft->Destroy();


			SetActorLocation(_StartingPos);
			_WaitTime = FMath::FRandRange(30.f, 120.f);
			_FirstTime = true;
			
			if (StationBP != NULL)
			{
				FActorSpawnParameters SpawnParams;
				_pStationLeft = GetWorld()->SpawnActor<AStation>(StationBP, StationLeftLocation, StationLeftRotation, SpawnParams);
				_pStationLeft->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				_pStationLeft->SpawnRandomDrops(10);

				_pStationRight = GetWorld()->SpawnActor<AStation>(StationBP, StationRightLocation, StationRightRotation, SpawnParams);
				_pStationRight->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
				_pStationRight->SpawnRandomDrops(10);
			}
			_IsRiding = false;
			IsLeaving = false;
			_DeleteTime = 20.f;
		}
	}
}

void AStationManager::ChangeToSplitsScreen(AActor* Context, bool bDisable)
{
	if (Context)
	{
		Context->GetWorld()->GetGameViewport()->SetDisableSplitscreenOverride(bDisable);
		GetWorld()->GetGameViewport()->UpdateActiveSplitscreenType();
	}
}

void AStationManager::SetScaleMultiplier(FVector scale)
{
	_ScaleMulti = scale;

}

