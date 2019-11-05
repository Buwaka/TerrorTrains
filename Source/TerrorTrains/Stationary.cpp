// Fill out your copyright notice in the Description page of Project Settings.

#include "Stationary.h"

// Sets default values
AStationary::AStationary()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//_Interable = CreateDefaultSubobject<UInteractable>(TEXT("Mount"));



	_Weapon = WeaponBP.GetDefaultObject();

	if (_Weapon == NULL)
		_Weapon = CreateDefaultSubobject<AWeapon>(TEXT("stationary Weapon"));
}

// Called when the game starts or when spawned
void AStationary::BeginPlay()
{
	Super::BeginPlay();

	//_Interable->InteractFunction->BindDynamic(this, &AStationary::Mount);

	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };
	_Weapon = GetWorld()->SpawnActor<AWeapon>(WeaponBP, Location, Rotation, SpawnInfo);
	
}

// Called every frame
void AStationary::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStationary::Mount(UObject * player)
{

	auto playerobj = Cast<APlayerCharacter>(player);
	if(playerobj == NULL)
	{
		UE_LOG(LogTemp, Error, TEXT("void AStationary::Mount(UObject * player), failed given player pointer is not a player"));
		return;
	}


//	playerobj->Freeze(true);
}




