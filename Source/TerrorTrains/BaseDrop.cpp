// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseDrop.h"
#include "SFXComponent.h"
#include "PlayerCharacter.h"

// Sets default values
ABaseDrop::ABaseDrop()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DropMesh = CreateDefaultSubobject<UStaticMeshComponent>("DropMesh");
	PickedUpSFX = CreateDefaultSubobject<USFXComponent>("SFX");
	DropMesh->SetCollisionProfileName("DropCollision");
	DropMesh->OnComponentBeginOverlap.AddDynamic(this, &ABaseDrop::OnOverlap);

	Cost = CreateDefaultSubobject<UMoneyComponent>("Cost");
}

// Called when the game starts or when spawned
void ABaseDrop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseDrop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseDrop::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto Player = OtherActor->GetComponentByClass(UMoneyComponent::StaticClass());
	if (Player != nullptr && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		auto PlayerMoneyComp = Cast<UMoneyComponent>(Player);
		if (PlayerMoneyComp != nullptr)
		{
			if (PlayerMoneyComp->CanAfford(Cost))
			{
				PlayerMoneyComp->SubstractMoney(Cost);
				Activate(OtherActor, SweepResult);
				PickedUpSFX->Play();
				if (ParticleBP != NULL)
				{
					GetWorld()->SpawnActor<AActor>(ParticleBP, GetActorLocation(), GetActorRotation());
				}
				Destroy();
			}
		}
	}
}


