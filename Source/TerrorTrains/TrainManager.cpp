// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainManager.h"
#include "Engine/World.h"
#include "TerrorTrainInstance.h"

// Sets default values
ATrainManager::ATrainManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



}

// Called when the game starts or when spawned
void ATrainManager::BeginPlay()
{
	Super::BeginPlay();

	TerrorTrainInstance->SetTrainManager(this);

	if (TrainBP != NULL)
	{
		if (_pTeam1 == nullptr)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			FAttachmentTransformRules aRules{ EAttachmentRule::KeepWorld,true };

			_pTeam1 = GetWorld()->SpawnActor<ATrain>(TrainBP, LocationTrain1, RotationTrain1, SpawnInfo);

			_pTeam1->SetTeam(1);
			_pTeam1->AttachToActor(this, aRules);
		}
	}
	if (TrainBP2 != NULL)
	{
		if (_pTeam2 == nullptr)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			FAttachmentTransformRules aRules{ EAttachmentRule::KeepWorld,true };

			_pTeam2 = GetWorld()->SpawnActor<ATrain>(TrainBP2, LocationTrain2, RotationTrain2, SpawnInfo);

			_pTeam2->SetTeam(2);
			_pTeam2->AttachToActor(this, aRules);
		}
	}
}

// Called every frame
void ATrainManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

ATrain* ATrainManager::GetTeam1()
{
	if (IsValid(_pTeam1))
	{
		return _pTeam1;
	}
	return nullptr;
}

ATrain* ATrainManager::GetTeam2()
{
	if (IsValid(_pTeam2))
	{
		return _pTeam2;
	}
	return nullptr;
}

void ATrainManager::AddTeam(FVector loc, FRotator rotation, FVector spaceInBetween)
{
	
}

void ATrainManager::AddWagon(int team)
{
	if (team == 1)
	{
		if (IsValid(_pTeam1))
		{
			_pTeam1->AddWagon();
		}
		return;
	}
	if (team == 2)
	{
		if (IsValid(_pTeam2))
		{
			_pTeam2->AddWagon();
		}
		return;
	}
}

void ATrainManager::SetMaxWagon(int maxwagon)
{
	if (IsValid(_pTeam1))
	{
		_pTeam1->SetMaxWagon(maxwagon);
	}
	if (IsValid(_pTeam2))
	{
		_pTeam2->SetMaxWagon(maxwagon);
	}
}

void ATrainManager::SetStartingWagon(int startingwagon)
{
	for (int i = 0; i < startingwagon; i++)
	{
		if (IsValid(_pTeam1))
		{
			_pTeam1->AddWagon();
		}
		if (IsValid(_pTeam2))
		{
			_pTeam2->AddWagon();
		}
	}
}

void ATrainManager::SetHealth(int team, float health)
{
	if (team == 1)
	{
		if (IsValid(_pTeam1))
		{
			_pTeam1->SetHealth(health);
		}
		return;
	}
	if (team == 2)
	{
		if (IsValid(_pTeam2))
		{
			_pTeam2->SetHealth(health);
		}
		return;
	}
}

void ATrainManager::SetTrainStop(bool Stop)
{
	if (IsValid(_pTeam1))
	{		
		_pTeam1->StopTrain(Stop);
	}
	if (IsValid(_pTeam2))
	{
		_pTeam2->StopTrain(Stop);
	}
}

void ATrainManager::DoRailsBreaking()
{
	if (IsValid(_pTeam1))
	{
		_pTeam1->DoRailsBreaking();
	}
	if (IsValid(_pTeam2))
	{
		_pTeam2->DoRailsBreaking();
	}
}

void ATrainManager::DoRailsNotBreaking()
{
	if (IsValid(_pTeam1))
	{
		_pTeam1->DoRailsNotBreaking();
	}
	if (IsValid(_pTeam2))
	{
		_pTeam2->DoRailsNotBreaking();
	}
}

void ATrainManager::SetTrainBreaking(bool Stop)
{
	if (IsValid(_pTeam1))
	{
		_pTeam1->StartBreaking(Stop);
	}
	if (IsValid(_pTeam2))
	{
		_pTeam2->StartBreaking(Stop);
	}
}

void ATrainManager::MoveOut(bool start, bool moveOut)
{
	if (IsValid(_pTeam1))
	{
		_pTeam1->MoveOut(start, MoveOutSpeed, moveOut);
	}
	if (IsValid(_pTeam2))
	{
		_pTeam2->MoveOut(start, -MoveOutSpeed, moveOut);
	}	
}

bool ATrainManager::GetDoneMoveOut()
{
	return (_pTeam1->GetDoneMoveOut() && _pTeam2->GetDoneMoveOut());
}

bool ATrainManager::TryBuyDoor(int team, APlayerCharacter* player)
{
	if (team == 1)
	{
		if (IsValid(_pTeam1))
		{
			return _pTeam1->BuyWagon(player);
		}
	}
	if (team == 2)
	{
		if (IsValid(_pTeam2))
		{
			return _pTeam2->BuyWagon(player);
		}
	}
	return false;
}

int ATrainManager::GetDoorCount(int team)
{
	int count{};

	switch (team)
	{
	case 1: count = GetTeam1()->GetNumOfWagons();
		break;
	case 2: count = GetTeam2()->GetNumOfWagons();
		break;
	}

	return count;
}

int ATrainManager::GetOpenDoorCount(int team)
{
	int count{};

	switch (team)
	{
	case 1: count = GetTeam1()->GetNumOfWagons() - GetTeam1()->GetNumOfDoorsLeft();
		break;
	case 2: count = GetTeam2()->GetNumOfWagons() - GetTeam2()->GetNumOfDoorsLeft();
		break;
	}

	return count;
}

