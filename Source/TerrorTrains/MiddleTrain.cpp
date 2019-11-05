// Fill out your copyright notice in the Description page of Project Settings.

#include "MiddleTrain.h"
#include "MiddleTrainWagon.h"
#include "Train.h"
#include "TrainWagon.h"
#include "Engine/World.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/Engine.h"
#include "EngineUtils.h"
#include "SFXComponent.h"

// Sets default values
AMiddleTrain::AMiddleTrain()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//pMesh = CreateDefaultSubobject<UStaticMeshComponent>("TrainMesh");
	pAnimationMesh = CreateDefaultSubobject<USkeletalMeshComponent>("TrainAnimationMesh");
	pAnim = CreateDefaultSubobject<UAnimSequence>("TrainAnim");
	RootComponent = pAnimationMesh;
	BackHitBox = CreateDefaultSubobject<UBoxComponent>("BackHitBox");
	trainLeavingSFX = CreateDefaultSubobject<USFXComponent>("SFX");
	BackHitBox->OnComponentBeginOverlap.AddDynamic(this, &AMiddleTrain::CheckIfTrainCanStop);
}

// Called when the game starts or when spawned
void AMiddleTrain::BeginPlay()
{
	Super::BeginPlay();

	_IsFirstTime = true;
	_BeginPos = this->GetActorLocation();
	_WaitTimer = FMath::FRandRange(10.f, 40.f);
	_CanRide = true;
	_MeshSize = pAnimationMesh->CalcBounds(FTransform()).BoxExtent * 3.1;
	pAnimationMesh->PlayAnimation(pAnim, true);
}

// Called every frame
void AMiddleTrain::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (_CanRide)
	{
		if (_WaitTimer <= 0)
		{
			if (!_IsOnPosition)
			{
				FVector temp = { this->GetActorLocation() };
				temp += TrainSpeed * DeltaTime;
				this->SetActorLocation(temp);
				FVector Location(GetActorLocation());
				//Setting the BoxComponent his location to the last wagon
				Location.Y -= _MeshSize.X + ((WagonBP.GetDefaultObject()->pMesh->CalcBounds(FTransform()).BoxExtent.X * 1.8)*_pWagons.Num());

				BackHitBox->SetWorldLocation(Location);
				if (!_IsFirstTime)
				{
					_DeleteTime -= DeltaTime;
					CheckIfTrainCanBeResetted();
				}
			}
			else
			{
				if(_Timer <= 5 && !_SFXHasPlayed)
				{
					trainLeavingSFX->Play();
					_SFXHasPlayed = true;
				}

				if (_Timer <= 0)
				{
					_IsOnPosition = false;
					_SFXHasPlayed = false;
				}
				_Timer -= DeltaTime;			
			}
			_IsRiding = true;
		}
		else
		{
			_WaitTimer -= DeltaTime;
		}
	}
}

void AMiddleTrain::CheckIfTrainCanStop(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (_IsFirstTime)
	{		
		if (OtherActor->IsA(ATrain::StaticClass()) || OtherActor->IsA(ATrainWagon::StaticClass()))
		{
			_IsOnPosition = true;
			_IsFirstTime = false;
		}
	}
}

void AMiddleTrain::CheckIfTrainCanBeResetted()
{
	if (_DeleteTime <= 0)
	{
		_IsRiding = false;
		for (auto wag : _pWagons)
		{
			wag->DeleteDrops();
			wag->Destroy();
		}
		Destroy();
		_DeleteTime = 20.f;
	}
}

void AMiddleTrain::IsStationRiding(bool canI)
{
	_CanRide = !canI;
}

bool AMiddleTrain::GetIsRide()
{
	return _IsRiding;
}

void AMiddleTrain::AddWagon(int num)
{

	if (WagonBP != NULL)
	{
		for (int i = 0; i < num; i++)
		{
			FVector Location(0, 0, 0);
			if (_pWagons.Num() == 0)
			{
				//Location just behind locomotive
				Location.X -= _MeshSize.X;
			}
			else
			{
				//Getting the WagonBP mesh his Dimensions and like above multiply with 2 because it gives half
				Location.X -= _MeshSize.X + ((WagonBP.GetDefaultObject()->pMesh->CalcBounds(FTransform()).BoxExtent.X * 2)*_pWagons.Num());
			}
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.Owner = this;
			FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };

			const int pos = _pWagons.Add(GetWorld()->SpawnActor<AMiddleTrainWagon>(WagonBP, Location, this->GetActorRotation(), SpawnInfo));

			_pWagons[pos]->AttachToActor(this, aRules);
			_pWagons[pos]->SpawnRandomDrops(5);
		}
	}

	FVector Location(GetActorLocation());
	//Setting the BoxComponent his location to the last wagon
	Location.Y -= _MeshSize.X + ((WagonBP.GetDefaultObject()->pMesh->CalcBounds(FTransform()).BoxExtent.X * 1.8)*_pWagons.Num());

	BackHitBox->SetWorldLocation(Location);
	BackHitBox->SetWorldRotation(GetActorRotation());
	BackHitBox->SetWorldScale3D(FVector(1, 100, 50));
}

