// Fill out your copyright notice in the Description page of Project Settings.

#include "MainCamera.h"
#include "UObjectIterator.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "EngineUtils.h"
#include "TerrorTrainInstance.h"
// Sets default values
AMainCamera::AMainCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = false;
}

// Called when the game starts or when spawned
void AMainCamera::BeginPlay()
{
	Super::BeginPlay();
	_PrevLocation = GetActorLocation();
	_GoToValue = GetActorLocation();
}

// Called every frame
void AMainCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	myCharactersTeam0 = TerrorTrainInstance->GetCharacterManager()->GetTeam(1);
	myCharactersTeam0.Append(TerrorTrainInstance->GetCharacterManager()->GetTeam(2));
	float lenghtY = 0;
	FVector CharacterLocation{};
	for (int i = 0; i < myCharactersTeam0.Num(); i++)
	{
		for (int k = 0; k < myCharactersTeam0.Num(); k++)
		{
			float floatTemp = 0;
			if (i != k)
			{
				if (IsValid(myCharactersTeam0[i]) && IsValid(myCharactersTeam0[k]))
				{
					floatTemp = abs(myCharactersTeam0[i]->GetActorLocation().Y - myCharactersTeam0[k]->GetActorLocation().Y);
					if (floatTemp > lenghtY)
					{
						lenghtY = floatTemp;
						if (myCharactersTeam0[i]->GetActorLocation().Y > myCharactersTeam0[k]->GetActorLocation().Y)
						{
							CharacterLocation = myCharactersTeam0[i]->GetActorLocation();
						}
						else { CharacterLocation = myCharactersTeam0[k]->GetActorLocation(); }
					}
				}
			}
		}
	}
	float Winninglenght{};
	float lenghtZ{};
	lenghtY *= 0.5f;
	Winninglenght = lenghtY;
	if (_FirstTime)
	{
		MaxDistance = Winninglenght;
		_FirstTime = false;
	}

	if (MaxDistance > Winninglenght)
	{
		Winninglenght = MaxDistance;
	}

	if (_PrevLenght != 0)
	{
		FVector vec = { 0.f,0.f,0.f };
		lenghtZ = Winninglenght - _PrevLenght;
		if (lenghtZ <= 0.f)
		{
			vec = GetActorForwardVector() * lenghtZ;
		}
		else
		{
			_PrevChanges += lenghtZ;
			if (_PrevChanges > MinDistanceChange)
			{
				vec = GetActorForwardVector() * _PrevChanges;
				_PrevChanges = 0.f;
			}
		}
		
		
		_GoToValue = GetActorLocation();
		_GoToValue -= vec;
		_GoToValue.Y = CharacterLocation.Y - lenghtY;
	}
	_PrevLocation = FMath::Lerp(_PrevLocation, _GoToValue, _TimeOfTranslate);
	SetActorLocation(_PrevLocation);
	_PrevLenght = Winninglenght;
}

