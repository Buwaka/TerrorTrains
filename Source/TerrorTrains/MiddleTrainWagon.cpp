// Fill out your copyright notice in the Description page of Project Settings.

#include "MiddleTrainWagon.h"
#include "TerrorTrainInstance.h"

// Sets default values
AMiddleTrainWagon::AMiddleTrainWagon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrainMesh");
	RootComponent = pMesh;
	_Drops = TArray<ABaseDrop*>();
}

// Called when the game starts or when spawned
void AMiddleTrainWagon::BeginPlay()
{
	Super::BeginPlay();
	_TrainRotation = GetActorRotation();
}

// Called every frame
void AMiddleTrainWagon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_Angle++;
	if (_Angle >= 360)
	{
		_Angle = 0;
	}

	_TrainRotation.Roll = _Offset * FMath::Sin(FMath::DegreesToRadians(_Angle* Speed));

	SetActorRotation(_TrainRotation);

}

void AMiddleTrainWagon::Destroyed()
{
	DeleteDrops();
}

void AMiddleTrainWagon::SpawnRandomDrops(int amount)
{
	_Drops = TerrorTrainInstance->GetDropManager()->SpawnRandomDrops(this, pMesh->CalcBounds(FTransform()).BoxExtent, amount,FVector(0,0,0));
}

void AMiddleTrainWagon::DeleteDrops()
{
	for (size_t i = 0; i < _Drops.Num(); i++)
	{
		if(_Drops[i]->IsValidLowLevel())
			_Drops[i]->Destroy();
	}
}

