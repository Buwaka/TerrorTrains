// Fill out your copyright notice in the Description page of Project Settings.
#include "CharacterManager.h"
#include "PlayerCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "TerrorTrainInstance.h"
#include "HealthComponent.h"
#include "MoneyComponent.h"
#include "AmmoComponent.h"
#include "DropManager.h"
#include "MoneyDrop.h"

// Sets default values
ACharacterManager::ACharacterManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACharacterManager::BeginPlay()
{
	Super::BeginPlay();
	TerrorTrainInstance->SetCharacterManager(this);
	RespawnTeam1.SetNum(_NumPlayersPerTeam);
	RespawnTeam2.SetNum(_NumPlayersPerTeam);

	/*if (PlayerBP1 != NULL && PlayerBP2 != NULL)
	{
		CreatePlayers();
	}*/
}

// Called every frame
void ACharacterManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckPlayers();

}

void ACharacterManager::CreatePlayers()
{
	//Get default controller from world
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), _pControllers);
	APlayerController* pDefaultController{};
	for(auto controller : _pControllers)
	{
		if(controller->IsA(APlayerController::StaticClass()))
		{
			pDefaultController = Cast<APlayerController>(controller);
			(*pDefaultController).UnPossess();
		}
	}

	//Create Teams - Assign default controller to player0 - Create new controllers for the other players
	for (int i{}; i < _NumPlayersPerTeam; ++i)
	{
		FVector Location(RespawnTeam1[i]);
		if (i != 0)
		{
			Location.Y += _pTeam1[0]->GetMesh()->CalcBounds(FTransform()).BoxExtent.Y + 300.f;
			RespawnTeam1[1] = Location;
		}
		
		FRotator Rotation(RespawnRotTeam1);
		FActorSpawnParameters SpawnInfo;
		FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };
		if (PlayerBP1[i] != NULL)
		{
			auto newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(PlayerBP1[i], Location, Rotation, SpawnInfo);
			if (i == 0)
			{
				newCharacter->SetPlayerIndex(PlayerIndex);
				newCharacter->SetTeam(1);
				//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green, FString::Printf(TEXT("Index of player: %f"), newCharacter->GetPlayerIndex()));
				PlayerIndex += 1;
				pDefaultController->Possess(newCharacter);
			}
			else
			{
				newCharacter->SetPlayerIndex(PlayerIndex);
				newCharacter->SetTeam(1);
				//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green, FString::Printf(TEXT("Index of player: %f"), newCharacter->GetPlayerIndex()));
				PlayerIndex += 1;
				UGameplayStatics::CreatePlayer(Cast<UObject>(GetWorld()), -1, true)->Possess(newCharacter);
				newCharacter->Money->SetTeamMate(_pTeam1[PlayerIndex - 2]->Money);
				_pTeam1[PlayerIndex - 2]->Money->SetTeamMate(newCharacter->Money);
			}
			auto num = _pTeam1.Add(newCharacter);
		}
	}


	for (int i{}; i < _NumPlayersPerTeam; ++i)
	{
		FVector Location(RespawnTeam2[i]);
		if (i != 0)
		{
			Location.Y += _pTeam2[0]->GetMesh()->CalcBounds(FTransform()).BoxExtent.Y + 300.f;
			RespawnTeam2[1] = Location;
		}
		FRotator Rotation(RespawnRotTeam2);
		FActorSpawnParameters SpawnInfo;
		FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };
		if (PlayerBP2[i] != NULL)
		{
			auto newCharacter = GetWorld()->SpawnActor<APlayerCharacter>(PlayerBP2[i], Location, Rotation, SpawnInfo);
			newCharacter->SetTeam(2);
			newCharacter->SetPlayerIndex(PlayerIndex);
			//GEngine->AddOnScreenDebugMessage(-1, 30.f, FColor::Green, FString::Printf(TEXT("Index of player: %i"), newCharacter->GetPlayerIndex()));
			PlayerIndex += 1;
			UGameplayStatics::CreatePlayer(Cast<UObject>(GetWorld()), -1, true)->Possess(newCharacter);
			_pTeam2.Add(newCharacter);
			if (i != 0)
			{
				newCharacter->Money->SetTeamMate(_pTeam2[PlayerIndex - 4]->Money);
				_pTeam2[PlayerIndex - 4]->Money->SetTeamMate(newCharacter->Money);
			}
		}
	}
}

void ACharacterManager::CheckPlayers()
{
	//Check team1 players
	if (_pTeam1.Num() != 0)
	{
		for(int i{}; i < _pTeam1.Num(); ++i)
		{
			if(_pTeam1[i] == nullptr)
			{
				_pTeam1.RemoveAt(i);
			}

			else if ((*_pTeam1[i]).IsRespawn())
			{
				Respawn(_pTeam1[i]);
			}
		}
	}

	//Check team2 players
	if (_pTeam2.Num() != 0)
	{
		for(int i{}; i < _pTeam2.Num(); ++i)
		{
			if(_pTeam2[i] == nullptr)
			{
				_pTeam2.RemoveAt(i);
			}

			else if ((*_pTeam2[i]).IsRespawn())
			{
				Respawn(_pTeam2[i]);
			}
		}
	}
}

void ACharacterManager::Respawn(APlayerCharacter*& pPlayerToRespawn)
{
	if ((*pPlayerToRespawn).GetTeam() == 1)
	{
		for (int i = 0; i < _NumPlayersPerTeam; i++)
		{
			if (_pTeam1[i] == pPlayerToRespawn)
			{
				//SpawnMoneyDrop(GetActorLocation(), pPlayerToRespawn->KllingMoney);
				GiveMoney(2, pPlayerToRespawn->KllingMoney);

				(*pPlayerToRespawn).SetActorLocation(RespawnTeam1[i]);
				(*pPlayerToRespawn).SetActorRotation(RespawnRotTeam1);
				(*pPlayerToRespawn).Health->SetInvincible(0);

				++TotalDeathsTeam1;
			}
		}
		
	}
	else if ((*pPlayerToRespawn).GetTeam() == 2)
	{

		for (int i = 0; i < _NumPlayersPerTeam; i++)
		{
			if (_pTeam2[i] == pPlayerToRespawn)
			{
				//SpawnMoneyDrop(GetActorLocation(), pPlayerToRespawn->KllingMoney);
				GiveMoney(1, pPlayerToRespawn->KllingMoney);

				(*pPlayerToRespawn).SetActorLocation(RespawnTeam2[i]);
				(*pPlayerToRespawn).SetActorRotation(RespawnRotTeam2);
				(*pPlayerToRespawn).Health->SetInvincible(0);
				++TotalDeathsTeam2;
			}
		}
	}
	(*pPlayerToRespawn).ResetRespawn();


}

void ACharacterManager::SetRespawnLocation(FVector respawnLoc, int TeamNum)
{
	if (TeamNum == 1)
	{
		for(int i{}; i < RespawnTeam1.Num(); ++i)
		{
			RespawnTeam1[i] = respawnLoc;
		}
		if (_pTeam1.Num() > 1)
		{
			RespawnTeam1[1].Y += _pTeam1[0]->GetMesh()->CalcBounds(FTransform()).BoxExtent.Y;
		}
	}
	else if (TeamNum == 2)
	{
		for(int i{}; i < RespawnTeam2.Num(); ++i)
		{
			RespawnTeam2[i] = respawnLoc;
		}
		if (_pTeam2.Num() > 1)
		{
			RespawnTeam2[1].Y += _pTeam2[0]->GetMesh()->CalcBounds(FTransform()).BoxExtent.Y;
		}
	}
}

void ACharacterManager::SetRespawnRotation(FRotator respawnRot, int TeamNum)
{
	if (TeamNum == 1)
	{
		RespawnRotTeam1 = respawnRot;
	}
	else if (TeamNum == 2)
	{
		RespawnRotTeam2 = respawnRot;
	}
}

TArray<APlayerCharacter*> ACharacterManager::GetTeam(int team)
{
	if (team == 1)
	{
		return _pTeam1;
	}
	if (team == 2)
	{
		return _pTeam2;
	}
	return TArray<APlayerCharacter*>();
}

void ACharacterManager::Reset()
{
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerController::StaticClass(), _pControllers);
	bool SkipFirst{};
	for(auto controller : _pControllers)
	{
		if(controller->IsA(APlayerController::StaticClass()))
		{
			auto pDefaultController = Cast<APlayerController>(controller);
			(*pDefaultController).UnPossess();
			if (!SkipFirst)
			{
				SkipFirst = true;
			}
			else
			{
				UGameplayStatics::RemovePlayer(pDefaultController,true);
			}
		
		}
	}
}

void ACharacterManager::GiveMoney(int team, float amount)
{
	switch (team)
	{
	case 1:  _pTeam1[0]->Money->AddMoneyFloat(amount);
		break;
	case 2: _pTeam2[0]->Money->AddMoneyFloat(amount);
		break;
	default:
		break;
	}
}

void ACharacterManager::RemoveMoney(int team, float amount)
{
	switch (team)
	{
	case 1:  _pTeam1[0]->Money->SubstractMoneyFloat(amount);
		break;
	case 2: _pTeam2[0]->Money->SubstractMoneyFloat(amount);
		break;
	default:
		break;
	}
}

float ACharacterManager::GetMoney(int team)
{
	float money{};
	if (team == 1)
	{
		for (auto play : _pTeam1)
		{
			money += play->Money->GetMoney();
		}
		return money;
	}

	if (team == 2)
	{
		for (auto play : _pTeam2)
		{
			money += play->Money->GetMoney();
		}
		return money;
	}
	return 0;
}

int ACharacterManager::GetTeamAmmo(int team)
{
	int Ammo{};
	if (team == 1)
	{
		for (auto play : _pTeam1)
		{
			Ammo += play->Ammo->GetAmmo();
		}
		return Ammo;
	}

	if (team == 2)
	{
		for (auto play : _pTeam2)
		{
			Ammo += play->Ammo->GetAmmo();
		}
		return Ammo;
	}
	return 0;

}

int ACharacterManager::GetAmmo(int team, int player)
{
	int ammo{};

	if (player >= _pTeam1.Num() && player >= _pTeam2.Num())
	{
		return ammo;
	}

	switch (team)
	{
	case 1: 
		ammo = 	_pTeam1[player]->Ammo->GetAmmo();
		break;
	case 2:
		ammo = _pTeam2[player]->Ammo->GetAmmo();
		break;
	}

	return ammo;
}

APlayerCharacter* ACharacterManager::GetPlayer(int team, int player)
{
	APlayerCharacter* temp = nullptr;

	if (player >= _pTeam1.Num() && player >= _pTeam2.Num())
	{
		return temp;
	}

	switch (team)
	{
	case 1:
		temp = _pTeam1[player];
		break;
	case 2:
		temp = _pTeam2[player];
		break;
	}

	return temp;
}

void ACharacterManager::SpawnMoneyDrop(FVector location, float amount)
{
	auto bdrop = TerrorTrainInstance->GetDropManager()->SpawnDrop(this, location, "MoneyDrop", false);
	if (bdrop)
	{
		auto mdrop = Cast<AMoneyDrop>(bdrop);
		if (mdrop)
		{
			mdrop->MoneyAmount = amount;
		}
	}
}

