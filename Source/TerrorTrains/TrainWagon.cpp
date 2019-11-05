// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainWagon.h"
#include "TrainWalls.h"
#include "Train.h"
#include "Engine/World.h"
#include "Components/StaticMeshComponent.h"

ATrainWagon::ATrainWagon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrainMesh");
	pMesh->OnComponentHit.AddDynamic(this, &ATrainWagon::OnTrainHit);
	RootComponent = pMesh;
}

// Called when the game starts or when spawned
void ATrainWagon::BeginPlay()
{
	Super::BeginPlay();
	_CurrentPos = _LastPos = this->GetActorLocation();
	_IsMoving = false;
	_FirstTime = true;
	_Timer = 0;
	_TrainRotation = GetActorRotation();
	for (int i = 0; i < WallLeftBP.Num(); i++)
	{
		if (WallLeftBP[i] != NULL)
		{
			//Create right wall and attach so actor will rotate with Wagon
			FVector Location(0.0f, 0.0f, 0.f);
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };

			auto num = _pWallLeft.Add(GetWorld()->SpawnActor<ATrainWalls>(WallLeftBP[i], Location, Rotation, SpawnInfo));

			Children.Add(_pWallLeft[num]);

			_pWallLeft[num]->Team = _Team;
			_pWallLeft[num]->pHealth->SetTeam(_Team);
			_pWallLeft[num]->AttachToActor(this, aRules);
		}
	}
	for (int i = 0; i < WallRightBP.Num(); i++)
	{
		if (WallRightBP[i] != NULL)
		{
			//Create right wall and attach so actor will rotate with Wagon
			FVector Location(0.0f, 0.0f, 0.f);
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo{};
			SpawnInfo.Owner = this;
			FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };

			auto num = _pWallRight.Add(GetWorld()->SpawnActor<ATrainWalls>(WallRightBP[i], Location, Rotation, SpawnInfo));

			Children.Add(_pWallRight[num]);

			_pWallRight[num]->Team = _Team;
			_pWallRight[num]->pHealth->SetTeam(_Team);
			_pWallRight[num]->AttachToActor(this, aRules);
		}
	}
	
	if (DoorBP != NULL)
	{
		FVector Location(0.0f, 0.0f, 0.f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo{};
		SpawnInfo.Owner = this;
		FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };

		pDoor = GetWorld()->SpawnActor<ADoor>(DoorBP, Location, Rotation, SpawnInfo);

		pDoor->AttachToActor(this, aRules);		
	}

	TArray<AActor*> childeren;
	this->GetAllChildActors(childeren, true);
	for (auto child : childeren)
	{
		if (child->IsA(ATrainWalls::StaticClass()))
		{
			_pWallRight.Push(Cast<ATrainWalls>(child));
		}
		
		if (child->IsA(AMoneyVault::StaticClass()))
		{
			_pMoneyVault = Cast<AMoneyVault>(child);
		}
	}
	
}

// Called every frame
void ATrainWagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Making the wagon rotate back and forward
	if (!_Stop)
	{
		_Angle++;
		if (_Angle >= 360)
		{
			_Angle = 0;
		}

		_TrainRotation.Roll = _Offset * FMath::Sin(FMath::DegreesToRadians(_Angle* Speed));

		SetActorRotation(_TrainRotation);
	}
}

void ATrainWagon::TranslateNextTrains(FVector newLocation)
{
	_IsMoving = true;
	//Moving every wagon to his prev wagon
	if (pNextTrain != nullptr)
	{
		pNextTrain->TranslateNextTrains(_CurrentPos);
	}
	_CurrentPos = newLocation;
}

void ATrainWagon::SetTeam(int team)
{
	_Team = team;
	for (auto wall : _pWallRight)
	{
		wall->Team = team;
		wall->pHealth->SetTeam(_Team);
	}
	for (auto wall : _pWallLeft)
	{
		wall->Team = team;
		wall->pHealth->SetTeam(_Team);
	}
	_pMoneyVault->Team = _Team;
}

void ATrainWagon::SetCurrentLocation()
{
	_CurrentPos = _LastPos = this->GetActorLocation();
}

int ATrainWagon::GetTeam()
{
	return _Team;
}

void ATrainWagon::StopTrain(bool stop)
{
	_Stop = stop;
}

void ATrainWagon::OnTrainHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	//if (OtherActor->IsA(ATrain::StaticClass()))
	//{
	//	auto tempActor = Cast<ATrain>(OtherActor);
	//	auto parentActor = Cast<ATrain>(GetParentActor());
	//	if (tempActor->pMesh == parentActor->pMesh)
	//	{
	//		_IsMoving = false;
	//	}
	//}
}

bool ATrainWagon::BuyDoor(APlayerCharacter* player)
{
	if (IsValid(pDoor) && player->GetTeam() == _Team)
	{
		return pDoor->BuyDoor(player);
	}
	return false;
}
