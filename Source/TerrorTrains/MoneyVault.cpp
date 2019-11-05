// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyVault.h"
#include "TerrorTrainInstance.h"
#include "Engine/Engine.h"
#include "MoneyDrop.h"

// Sets default values
AMoneyVault::AMoneyVault()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrainMesh");
	pMesh->OnComponentHit.AddDynamic(this, &AMoneyVault::OnHit);
	RootComponent = pMesh;
}

// Called when the game starts or when spawned
void AMoneyVault::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void AMoneyVault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMoneyVault::DropMoney(float Damage)
{
	auto teamVec = TerrorTrainInstance->GetCharacterManager();
	DropLocation = { 0,0,0 };
	DropLocation.X = -pMesh->CalcBounds(FTransform()).BoxExtent.X * 2;
	while (true)
	{
		if (Damage >= MoneyPerDrop)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Money: %f"), MoneyPerDrop));
			if (teamVec->GetMoney(Team) <= 0)
			{
				break;
			}

			auto drop = TerrorTrainInstance->GetDropManager()->SpawnDrop(this, DropLocation, "MoneyDrop");

			if (IsValid(drop))
			{
				Cast<AMoneyDrop>(drop)->MoneyAmount = MoneyPerDrop;
			}
			if (IsValid(teamVec))
			{
				if (teamVec->GetMoney(Team) >= MoneyPerDrop)
				{
					teamVec->RemoveMoney(Team,MoneyPerDrop);
				}
				else
				{
					teamVec->RemoveMoney(Team, teamVec->GetMoney(Team));
					Cast<AMoneyDrop>(drop)->MoneyAmount = teamVec->GetMoney(Team);
					break;
				}
			}
			DropLocation.Y += pMesh->CalcBounds(FTransform()).BoxExtent.Y;
			
			Damage -= MoneyPerDrop;
			if (Damage <= 0.f)
			{
				break;
			}
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Money: %f"), Damage));
			if (teamVec->GetMoney(Team) <= 0)
			{
				break;
			}
			auto drop = TerrorTrainInstance->GetDropManager()->SpawnDrop(this, DropLocation, "MoneyDrop");
			if (IsValid(drop))
			{
				Cast<AMoneyDrop>(drop)->MoneyAmount = Damage;
			}
			if (IsValid(teamVec))
			{
				if (teamVec->GetMoney(Team) >= Damage)
				{
					teamVec->RemoveMoney(Team, Damage);
				}
				else
				{
					teamVec->RemoveMoney(Team, teamVec->GetMoney(Team));
					Cast<AMoneyDrop>(drop)->MoneyAmount = teamVec->GetMoney(Team);
					break;
				}
			}
			Damage = 0;
			break;
		}
	}
}

void AMoneyVault::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (OtherActor->IsA(ABullet::StaticClass()))
		{
			auto actor =  Cast<ABullet>(OtherActor);
			if (actor->GetTeam() != Team)
			{
				DropMoney(actor->Damage);
			}
			return;
		}
		if (OtherActor->IsA(AMeleeWeapon::StaticClass()))
		{
			auto actor = Cast<AMeleeWeapon>(OtherActor);
			if (actor->GetTeam() != Team)
			{
				DropMoney(actor->DefaultDamage);
			}
		}
	}
}


