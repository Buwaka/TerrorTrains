// Fill out your copyright notice in the Description page of Project Settings.

#include "Station.h"
#include "UObjectIterator.h"
#include "Engine/Engine.h"
#include "MiddleTrain.h"
#include "EngineUtils.h"
#include "Train.h"
#include "TrainWagon.h"
#include "Components/StaticMeshComponent.h"
#include "TerrorTrainInstance.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

// Sets default values
AStation::AStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pStationMesh = CreateDefaultSubobject<UStaticMeshComponent>("StationMesh");
	RootComponent = pStationMesh;
}

// Called when the game starts or when spawned
void AStation::BeginPlay()
{
	Super::BeginPlay();

}

void AStation::Destroyed()
{
	TArray<AActor*> temp;
	GetAttachedActors(temp);


	for (AActor* Actor : temp)
	{
		Actor->Destroy();
	}

	Super::Destroyed();
}

// Called every frame
void AStation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void AStation::SpawnRandomDrops(int amount)
{
	//haha pivots amirite
	auto trans = this->GetTransform();
	trans.SetRotation(FTransform().GetRotation());
	auto box = pStationMesh->CalcBounds(FTransform()).BoxExtent;

	TerrorTrainInstance->GetDropManager()->SpawnRandomDrops(this, box , amount, Offset);
}

void AStation::DeleteDrops()
{
	//not necessary, Dropmanager takes care
}

