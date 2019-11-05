// Fill out your copyright notice in the Description page of Project Settings.

#include "Door.h"
#include "SFXComponent.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pMesh = CreateDefaultSubobject<UStaticMeshComponent>("DoorMesh");
	HitBox = CreateDefaultSubobject<UBoxComponent>("HitBox");
	pOpenDoorSFX = CreateDefaultSubobject<USFXComponent>("SFX");
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &ADoor::OnOverlap);
	HitBox->OnComponentEndOverlap.AddDynamic(this, &ADoor::OnOverlapEnd);
	pCost = CreateDefaultSubobject<UMoneyComponent>("Cost");

	RootComponent = pMesh;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	//HitBox->SetWorldLocation(GetActorLocation());
	//HitBox->SetWorldRotation(GetActorRotation());
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HitBox->SetWorldLocation(pMesh->GetComponentLocation());
}

void ADoor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		_CanBuy.Add(true);
	}
}

void ADoor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		_CanBuy.Pop();
	}
}

bool ADoor::BuyDoor(APlayerCharacter* player)
{
	for (auto buy : _CanBuy)
	{
		if (buy)
		{
			if (player->Money->CanAfford(pCost))
			{
				player->Money->SubstractMoney(pCost);
				pOpenDoorSFX->Play();
				if (BoughtParticleBP != NULL)
				{
					GetWorld()->SpawnActor<AActor>(BoughtParticleBP, GetActorLocation(), GetActorRotation());
				}
				if (IsValid(m_Particle))
				{
					m_Particle->Destroy();
				}
				Destroy();
				return true;
			}
		}
	}
	return false;
}

bool ADoor::CanBuyDoor()
{
	for (auto buy : _CanBuy)
	{
		if (buy)
		{
			return true;
		}
	}
	return false;
}

void ADoor::DoAfford()
{
	if (CanBuyParticleBP != NULL && !_Particle)
	{
		auto DoorLocation = GetActorLocation();
		DoorLocation.Z += 300.f;
		m_Particle = GetWorld()->SpawnActor<AActor>(CanBuyParticleBP, DoorLocation, GetActorRotation());
		m_Particle->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		_Particle = true;
	}
}

