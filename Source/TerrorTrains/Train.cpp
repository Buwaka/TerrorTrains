// Fill out your copyright notice in the Description page of Project Settings.

#include "Train.h"
#include "TrainWagon.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "TerrorTrainInstance.h"
// Sets default values
ATrain::ATrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//pMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrainMesh");
	pAnimationMesh = CreateDefaultSubobject<USkeletalMeshComponent>("TrainAnimationMesh");
	pAnim = CreateDefaultSubobject<UAnimSequence>("TrainAnim");
	RootComponent = pAnimationMesh;
}

// Called when the game starts or when spawned
void ATrain::BeginPlay()
{
	Super::BeginPlay();
	_TrainLocation = GetActorLocation();
	_LastTrainLocation = _OGTrainLocation = GetActorLocation();
	_GoToLocation = GetActorLocation() - LocationOffset;
	//Calc the mesh his size and multi normally with 2 because it gives back the half lenght but this mesh is weird so it needs 2.55f
	_MeshBounds = pAnimationMesh->CalcBounds(FTransform()).BoxExtent * 2.5f;
	_Speed = FMath::FRandRange(0.5, 1.f);
	_Offset = FMath::FRandRange(150.f, 300.f);
	if (RailsBP != NULL)
	{
		_Rails = GetWorld()->SpawnActor<ARailsTrackGenaration>(RailsBP, { 0,0,0 }, { 0,0,0 });
	}
	AddWagon();
	_Rails->KeepMakingRails();
	_Rails->StopRailsMoving(false);
	_Rails->SetLifeTime(30.f);

	TArray<AActor*> childeren;
	this->GetAllChildActors(childeren, true);
	for (auto child : childeren)
	{
		if (child->IsA(ADoor::StaticClass()))
		{
			pChest = Cast<ADoor>(child);
		}
	}
	pAnimationMesh->PlayAnimation(pAnim, true);
}

// Called every frame
void ATrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_WagonsAlive <= 0)
	{
		Destroy();
	}
	_Time++;
	if (!_Stop)
	{
		FVector tempLocOffset{};

		tempLocOffset = _TrainLocation;

		tempLocOffset.Y += _Offset * FMath::Sin(FMath::DegreesToRadians(_Time* _Speed));
		_GoToLocation.Y = tempLocOffset.Y;

		SetActorLocation(tempLocOffset);
	}
	
	
}

void ATrain::SetTeam(int team)
{
	_Team = team;
	for (int i = 0; i < _pWagons.Num(); i++)
	{
		_pWagons[i]->SetTeam(_Team);
	}
}

int ATrain::GetTeam()
{
	return _Team;
}

void ATrain::AddWagon()
{
	int max = (WagonBP.Num() - 1);
	int randWagon = FMath::RandRange(0, max);
	if (_UsedWagons.Num() == 0)
	{
		_UsedWagons.Add(randWagon);
	}
	else
	{
		if (_UsedWagons.Num() == WagonBP.Num())
		{
			_UsedWagons.Empty();
		}
		bool done{ false };
		do
		{
			done = false;
			randWagon = FMath::RandRange(0, max);
			for (int num : _UsedWagons)
			{
				if (num == randWagon)
				{
					done = true;
				}
			}
		} while (done);
		_UsedWagons.Add(randWagon);
	}
	if (WagonBP[randWagon] != NULL && _pWagons.Num() != _MaxNumberOfWagons)
	{
		_WagonsAlive++;
		_DoorsLeft++;
		FVector Location(0.0f, 0.0f, 0.f);
		if (_pWagons.Num() == 0)
		{
			//Location just behind locomotive
			if (IsValid(_Rails))
			{
				_Rails->MakeRails(GetActorLocation(), GetActorRotation(), 2);
			}
			Location.X -= _MeshBounds.X;
		}
		else
		{
			//Getting the WagonBP mesh his Dimensions and like above multiply with 2 because it gives half
			if (IsValid(_Rails))
			{
				_Rails->MakeRailsAtEnd(2);
			}
			Location.X -= _MeshBounds.X + ((WagonBP[randWagon].GetDefaultObject()->pMesh->CalcBounds(FTransform()).BoxExtent.X * 2)*_pWagons.Num());
		}
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };

		auto num = _pWagons.Add(GetWorld()->SpawnActor<ATrainWagon>(WagonBP[randWagon], Location, this->GetActorRotation(), SpawnInfo));

		_pWagons[num]->SetOwner(this);
		_pWagons[num]->SetTeam(_Team);
		_pWagons[num]->AttachToActor(this, aRules);
		_pWagons[num]->SetCurrentLocation();
		_pActiveWagon = _pWagons[num];

		if (IsValid(_pWagons[num]) && IsValid(_pWagons[num]->pDoor->pCost))
		{
			_pWagons[num]->pDoor->pCost->SetMoney(StartDoorCost);
			DoorArr.Add(_pWagons[num]->pDoor);
		}

		if (num == 0)
		{
			return;
		}
		else
		{
			_pWagons[num]->pNextTrain = _pWagons[num - 1];
			_pWagons[num - 1]->pPrevTrain = _pWagons[num];
		}

		for (auto wagon : _pWagons)
		{
			if (wagon != _pWagons[num])
			{
				if (IsValid(wagon) && IsValid(wagon->pDoor->pCost))
				{
					wagon->pDoor->pCost->AddMoneyFloat(CostIncrease);
				}
			}
		}
	}
}

void ATrain::SetMaxWagon(int maxwagon)
{
	_MaxNumberOfWagons = maxwagon;
}

int ATrain::GetMaxWagon()
{
	return _MaxNumberOfWagons;
}

int ATrain::GetNumOfWagons()
{
	return _WagonsAlive;
}

void ATrain::DeleteWagon()
{
	_WagonsAlive--;
}

void ATrain::StopTrain(bool stop)
{
	_Stop = stop;
	_Rails->StopRailsMoving(stop);
	for (auto wag : _pWagons)
	{
		if (wag)
		{
			wag->StopTrain(stop);			
		}
	}
	if (stop && stop != _PrevStop)
	{
		pAnimationMesh->OverrideAnimationData(pAnim, false, false);
	}
	else { pAnimationMesh->OverrideAnimationData(pAnim, true, true); }
	_PrevStop = stop;
}

void ATrain::DoRailsBreaking()
{
	_Rails->SlowDown();
}

void ATrain::DoRailsNotBreaking()
{
	_Rails->BackToNormalSpeed();
}

bool ATrain::GetIsStopped()
{
	return _Stop;
}

void ATrain::StartBreaking(bool breaking)
{
	_Breaking = breaking;
}

bool ATrain::GetIsBreaking()
{
	return _Breaking;
}

void ATrain::MoveOut(bool start, FVector dir, bool moveOut)
{
	_MoveOut = start;
	if (_MoveOut && _FirstTime)
	{
		if (moveOut)
		{
			_MoveDir = dir;
			_GoBack = false;
		}
		else
		{
			_GoBack = true;
		}
		_FirstTime = false;
	}
	else
	{
		_FirstTime = true;
		_DoneTranslate = false;
	}
	
}

bool ATrain::GetDoneMoveOut()
{
	return _DoneTranslate;
}

bool ATrain::BuyWagon(APlayerCharacter* player)
{
	if (!IsValid(_pActiveWagon))
	{
		if (IsValid(pChest))
		{
			if (player->GetTeam() == _Team)
			{
				if (pChest->BuyDoor(player))
				{
					TerrorTrainInstance->GetOverworldManager()->DoWin(_Team);
					return false;
				}
			}
		}
		else { return false; }
	}
	if (_pActiveWagon->BuyDoor(player))
	{
		_pActiveWagon->_pMoneyVault->Destroy();
		_pActiveWagon = _pActiveWagon->pNextTrain;
		_DoorsLeft--;
		//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("DoorsLeft: %f"), _DoorsLeft));
		return true;
	}
	return false;
}

void ATrain::CanAfford(float money)
{
	if (IsValid(_pActiveWagon))
	{
		if (!_pActiveWagon->pDoor->pCost->CanAffordFloat(money))
		{
			_pActiveWagon->pDoor->DoAfford();
		}
	}
}

bool ATrain::AllowedToBuy()
{
	if (!IsValid(_pActiveWagon))
	{
		if (IsValid(pChest))
		{
			return pChest->CanBuyDoor();
		}
		else { return false; }
	}
	return _pActiveWagon->pDoor->CanBuyDoor();
}

int ATrain::GetNumOfDoorsLeft()
{
	return _DoorsLeft;
}

void ATrain::SetHealth(float health)
{
	for (int i = 0; i < _pWagons.Num(); i++)
	{
		_pWagons[i]->pHealth->DefaultHealth = health;
	}
}

ATrainWagon* ATrain::GetActiveWagon()
{
	return _pActiveWagon;
}

ARailsTrackGenaration* ATrain::GetTrackGenerator()
{
	return _Rails;
}

