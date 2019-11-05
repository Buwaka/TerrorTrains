// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyDrop.h"
#include "PlayerCharacter.h"
#include "MoneyComponent.h"

AMoneyDrop::AMoneyDrop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = DropMesh;
	Cost->SetMoney(0);
}

// Called when the game starts or when spawned
void AMoneyDrop::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMoneyDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMoneyDrop::Activate(AActor* OtherActor, const FHitResult& Hit)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		auto ptr = OtherActor->GetComponentByClass(UMoneyComponent::StaticClass());
		if (ptr != nullptr)
		{
			auto MoneyComp = Cast<UMoneyComponent>(ptr);
			if (MoneyComp != nullptr)
			{
				MoneyComp->AddMoneyFloat(MoneyAmount);
			}
		}
		Destroy();
	}
}