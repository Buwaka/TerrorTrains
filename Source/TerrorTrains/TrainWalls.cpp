// Fill out your copyright notice in the Description page of Project Settings.

#include "TrainWalls.h"
#include "Components/StaticMeshComponent.h"
#include "SFXComponent.h"
ATrainWalls::ATrainWalls()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrainMesh");
	pHealth = CreateDefaultSubobject<UHealthComponent>("WallsHealth");
	HitSFX =  CreateDefaultSubobject<USFXComponent>("HitSFX");
	DestroySFX = CreateDefaultSubobject<USFXComponent>("DestroySFX");
	pKillMoney = CreateDefaultSubobject<UMoneyComponent>("KillMoney");

	pKillMoney->DefaultKillMoney = 50.f;
	RootComponent = pMesh;

	//HitSFX->Volume = Volume;
}

// Called when the game starts or when spawned
void ATrainWalls::BeginPlay()
{
	Super::BeginPlay();
	//HitSFX->Volume = Volume;

	auto children  = RootComponent->GetAttachChildren();
	for (auto child : children)
	{
		if (child->IsA(UStaticMeshComponent::StaticClass()))
		{
			auto num = pMeshes.Add(Cast<UStaticMeshComponent>(child));
			pMeshes[num]->SetVisibility(false);
		}
	}
	Algo::Reverse(pMeshes);
	m_HealthPercentage = pHealth->GetHealth() / float(pMeshes.Num()+1);
	m_HealthChange = pHealth->GetHealth() - m_HealthPercentage;


}

// Called every frame
void ATrainWalls::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (pHealth->GetHealth() <= 0)
	{
		DestroySFX->Play();
		Destroy();
		if (DestroyParticleBP != NULL)
		{
			GetWorld()->SpawnActor<AActor>(DestroyParticleBP, GetActorLocation(), GetActorRotation());
		}
	}
	if (pHealth->GetHealth() <= m_HealthChange && pMeshes.Num() != 0)
	{
		m_HealthChange -= m_HealthPercentage;
		pMesh->SetVisibility(false);
		pMesh = pMeshes.Last();
		pMeshes.Pop();
		pMesh->SetVisibility(true);
	}
}

