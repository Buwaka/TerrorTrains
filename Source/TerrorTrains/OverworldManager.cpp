// Fill out your copyright notice in the Description page of Project Settings.

#include "OverworldManager.h"
#include "UObjectIterator.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "TerrorTrainInstance.h"
#include "Kismet/GameplayStatics.h"
#include "TrainWagon.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AOverworldManager::AOverworldManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOverworldManager::BeginPlay()
{
	Super::BeginPlay();
	TerrorTrainInstance->SetOverworldManager(this);
	BGM->SFX->bLooping = true;
	BGM->Play();
	if (BPTrack != NULL)
	{
		_pTrackGen = GetWorld()->SpawnActor<ARailsTrackGenaration>(BPTrack, { 0,0,0 }, { 0,0,0 });
	}
	if (IsValid(_pTrackGen))
	{
		_pTrackGen->MakeRails(GroundLoc, GroundRot, NrOfStartObjects,true);
		_pTrackGen->KeepMakingRails();
		_pTrackGen->StopRailsMoving(false);
		_pTrackGen->SetLifeTime(0.f);
		_pTrackGen->Offset = GroundSpawnOffset;
	}
	if(wEOGMenu)
	{
		MyEOGMenu = CreateWidget<UUserWidget>(this->GetGameInstance(),wEOGMenu);
	}
}

// Called every frame
void AOverworldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_GameTime += DeltaTime;
	if (_FirstTime)
	{
		_pTrainManager = TerrorTrainInstance->GetTrainManager();

		_pCharManager = TerrorTrainInstance->GetCharacterManager();

		_pStationManager = TerrorTrainInstance->GetStationManager();

		_FirstTime = false;

		_pTrainManager->SetStartingWagon(NumOfWagons);

		if (IsValid(_pCharManager))
		{
			ATrainWagon* tempWagonTeam01 = _pTrainManager->GetTeam1()->GetActiveWagon();
			ATrainWagon* tempWagonTeam02 = _pTrainManager->GetTeam2()->GetActiveWagon();
			if (IsValid(tempWagonTeam01))
			{
				auto loc = tempWagonTeam01->GetActorLocation();
				loc.Z += tempWagonTeam01->pMesh->CalcBounds(FTransform()).BoxExtent.Z*2.f;
				loc.X += tempWagonTeam01->pMesh->CalcBounds(FTransform()).BoxExtent.X / 10.f;
				_pCharManager->SetRespawnLocation(loc, 1);
				_pCharManager->SetRespawnRotation({ 0,0,0 }, 1);
			}
			if (IsValid(tempWagonTeam02))
			{
				auto loc = tempWagonTeam02->GetActorLocation();
				loc.Z += tempWagonTeam02->pMesh->CalcBounds(FTransform()).BoxExtent.Z*2.f;
				loc.X += tempWagonTeam02->pMesh->CalcBounds(FTransform()).BoxExtent.X / 10.f;
				_pCharManager->SetRespawnLocation(loc, 2);
				_pCharManager->SetRespawnRotation({ 0.f,0.f,0.f }, 2);
			}
			_pCharManager->CreatePlayers();
		}
	}
	if (!IsValid(_pMiddleTrain))
	{
		if (BPMiddleTrain != NULL)
		{
			_pMiddleTrain = GetWorld()->SpawnActor<AMiddleTrain>(BPMiddleTrain, MiddleTrainLoc, MiddleTrainRot, FActorSpawnParameters());
			int numOfTeam1 = _pTrainManager->GetTeam1()->GetNumOfWagons();
			int numOfTeam2 = _pTrainManager->GetTeam2()->GetNumOfWagons();
			int numOfTeam2Wagons = numOfTeam1 + abs(numOfTeam1 - numOfTeam2);
			_pMiddleTrain->AddWagon(numOfTeam2Wagons);			
		}
	}
	else
	{
		CheckMiddleTrain();
		CheckStation();
	}		
	CheckBuyDoor();
	CheckWinOfTeam(1);
	CheckWinOfTeam(2);
	_pTrainManager->GetTeam1()->CanAfford(_pCharManager->GetMoney(1));
	_pTrainManager->GetTeam2()->CanAfford(_pCharManager->GetMoney(2));
}

void AOverworldManager::StartGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), "MainTrainLevel");
}

void AOverworldManager::PauseGame()
{

}

void AOverworldManager::ResetGame()
{
	_pCharManager->Reset();
}

void AOverworldManager::DoWin(int team)
{
	if (team == 1)
	{
		_Team1Wins = true;
	}

	if (team == 2)
	{
		_Team2Wins = true;
	}
}

int AOverworldManager::GetNumberOfWagons(int team)
{
	if (team == 1)
	{
		return _pTrainManager->GetTeam1()->GetNumOfWagons();
	}

	if (team == 2)
	{
		return _pTrainManager->GetTeam2()->GetNumOfWagons();
	}
	return 50000;
}

float AOverworldManager::GetMoney(int team)
{
	return _pCharManager->GetMoney(team);
}

bool AOverworldManager::CheckWinOfTeam(int team)
{
	if (team == 1)
	{
		if (_Team1Wins)
		{
			if(MyEOGMenu)
			{
				BGM->Pause();
				auto controller = Cast<APlayerController>(_pCharManager->GetTeam(1)[0]->GetController());
				controller->bShowMouseCursor = true;
				WinQuote = "Team 1 won the game";
				MyEOGMenu->AddToViewport();
				eogSFX->Play();
				UGameplayStatics::SetGamePaused(this->GetWorld(),true);
			}
		}
		
		return _Team1Wins;
	}
	if (team == 2)
	{
		if (_Team2Wins)
		{
			if(MyEOGMenu)
			{
				BGM->Pause();
				MyEOGMenu->AddToViewport();
				auto controller = Cast<APlayerController>(_pCharManager->GetTeam(1)[0]->GetController());
				WinQuote = "Team 2 won the game";
				controller->bShowMouseCursor = true;
				eogSFX->Play();
				UGameplayStatics::SetGamePaused(this->GetWorld(),true);
			}
		}
		return _Team2Wins;
	}
	return false;
}

float AOverworldManager::GetGameTime()
{
	return _GameTime;
}

int AOverworldManager::GetTeamDeaths(int team)
{
	if (team == 1)
	{
		return _pCharManager->TotalDeathsTeam1;
	}
	if (team == 2)
	{
		return _pCharManager->TotalDeathsTeam2;
	}

	return 0;
}

int AOverworldManager::GetRemainingDoors()
{
	return _pTrainManager->GetTeam1()->GetNumOfDoorsLeft() + _pTrainManager->GetTeam2()->GetNumOfDoorsLeft();
}

void AOverworldManager::CheckMiddleTrain()
{
	if (_pMiddleTrain->GetIsRide())
	{
		if (!_FirstTimeRiding)
		{
			_FirstTimeRiding = true;
			_pTrainManager->GetTeam1()->GetTrackGenerator()->DoSplit();
		}
		_pStationManager->IsMiddleTrainRiding(true);
	}
	else
	{
		if (_FirstTimeRiding)
		{
			_pTrainManager->GetTeam1()->GetTrackGenerator()->DoSplit(true);
			_FirstTimeRiding = false;
		}		
		_pStationManager->IsMiddleTrainRiding();
	}
}

void AOverworldManager::CheckStation()
{
	if (_pStationManager->GetIsRiding() || _pStationManager->GetIsArrived())
	{
		_pMiddleTrain->IsStationRiding(true);
		if (_pStationManager->GetIsArrived())
		{
			_pTrainManager->SetTrainStop(true);
			if (IsValid(_pTrackGen))
			{
				_pTrackGen->StopRailsMoving(true);
			}
			_pTrainManager->SetTrainBreaking(false);
		}
		else
		{
			_pTrainManager->SetTrainStop(false);
			if (IsValid(_pTrackGen))
			{
				_pTrackGen->StopRailsMoving(false);
			}
			if (!_pStationManager->IsLeaving)
			{
				_pTrainManager->SetTrainBreaking();
				if (IsValid(_pTrackGen))
				{
					_pTrackGen->SlowDown();
				}
				_pTrainManager->DoRailsBreaking();
			}
		}		
	}
	else 
	{
		_pMiddleTrain->IsStationRiding();
		int numOfTeam1 = _pTrainManager->GetTeam1()->GetNumOfWagons();
		int numOfTeam2 = _pTrainManager->GetTeam2()->GetNumOfWagons();
		int numOfTeam2Wagons = numOfTeam1 + abs(numOfTeam1 - numOfTeam2);
		_pStationManager->SetScaleMultiplier(FVector{ float(numOfTeam2Wagons),1.f,1.f });
		_pTrackGen->BackToNormalSpeed();
		_pTrainManager->DoRailsNotBreaking();
	}
}

void AOverworldManager::CheckBuyDoor()
{
	auto temp = _pCharManager->GetTeam(1);
	for (auto character : temp)
	{
		if (character->WantsToInteract() && _pTrainManager->GetTeam1()->AllowedToBuy())
		{
			if (_pTrainManager->TryBuyDoor(character->GetTeam(), character))
			{
				if (IsValid(_pCharManager))
				{
					ATrainWagon* tempWagonTeam01 = _pTrainManager->GetTeam1()->GetActiveWagon();
					if (IsValid(tempWagonTeam01))
					{
						auto loc = tempWagonTeam01->GetActorLocation();
						loc.Z += tempWagonTeam01->pMesh->CalcBounds(FTransform()).BoxExtent.Z*2.f;
						loc.X += tempWagonTeam01->pMesh->CalcBounds(FTransform()).BoxExtent.X / 10.f;
						_pCharManager->SetRespawnLocation(loc, 1);
						_pCharManager->SetRespawnRotation({ 0,0,0 }, 1);
					}
				}
			}
		}		
	}
	temp = _pCharManager->GetTeam(2);
	for (auto character : temp)
	{
		if (character->WantsToInteract() && _pTrainManager->GetTeam2()->AllowedToBuy())
		{
			if (_pTrainManager->TryBuyDoor(character->GetTeam(), character))
			{
				if (IsValid(_pCharManager))
				{
					ATrainWagon* tempWagonTeam01 = _pTrainManager->GetTeam2()->GetActiveWagon();
					if (IsValid(tempWagonTeam01))
					{
						auto loc = tempWagonTeam01->GetActorLocation();
						loc.Z += tempWagonTeam01->pMesh->CalcBounds(FTransform()).BoxExtent.Z*2.f;
						loc.X += tempWagonTeam01->pMesh->CalcBounds(FTransform()).BoxExtent.X / 10.f;
						_pCharManager->SetRespawnLocation(loc, 2);
						_pCharManager->SetRespawnRotation({ 0,0,0 }, 2);
					}
				}
			}
		}
	}
}