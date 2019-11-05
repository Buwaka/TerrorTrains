// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "HealthComponent.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"
#include "PlayerCharacter.h"
#include "TrainWagon.h"
#include "TrainWalls.h"
#include "PlayerCharacterController.h"
#include "SoundManager.h"
#include "TerrorTrainInstance.h"
#include "SFXComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	pBulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletMesh");
	pBulletMesh->SetCollisionProfileName("BulletCollision");
	pBulletMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	RootComponent = pBulletMesh;


	/*pBulletSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BulletSkeletalMesh");
	pBulletSkeletalMesh->OnComponentHit.AddDynamic(this, &ABullet::OnHit);
	pBulletSkeletalMesh->SetCollisionProfileName("BulletCollision");*/

	pHitParticle = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");

	pExplosiveRadius = CreateDefaultSubobject<USphereComponent>("Sphere");
	pExplosiveRadius->OnComponentEndOverlap.AddDynamic(this , &ABullet::CheckExplosiveRadius);
	pExplosiveRadius->SetSphereRadius(Radius);
	

	_ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	
	_ProjectileMovementComponent->SetUpdatedComponent(pBulletMesh);
	_ProjectileMovementComponent->InitialSpeed = BulletSpeed;
	_ProjectileMovementComponent->MaxSpeed = FLT_MAX;
	_ProjectileMovementComponent->bRotationFollowsVelocity = true;
	_ProjectileMovementComponent->bShouldBounce = false;
	_ProjectileMovementComponent->ProjectileGravityScale = 0;

	ExplosionSFX = CreateDefaultSubobject<USFXComponent>("SFX");
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	//TerrorTrainInstance->GetSoundManager()->AddSound(this, HitPlayerSFX);
//	TerrorTrainInstance->GetSoundManager()->AddSound(this, HitWallSFX);
	//if (Type == BulletType::Explosive)
	//{
	//	pBulletMesh->DestroyComponent();
	//	RootComponent = pBulletSkeletalMesh;
	//	//_ProjectileMovementComponent->SetUpdatedComponent(pBulletSkeletalMesh);
	//}
	//else
	//{
	//	pBulletSkeletalMesh->DestroyComponent();
	//}

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	_Timer += DeltaTime;
	if (_Timer >= LifeTime)
	{
		Destroy();
	}
	pExplosiveRadius->SetWorldLocation(GetActorLocation());

}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		auto ptr = OtherActor->GetComponentByClass(UHealthComponent::StaticClass());
		if (ptr != nullptr)
		{
			auto healthComp = Cast<UHealthComponent>(ptr);

			switch (Type)
			{
			case BulletType::Simple:

				if (healthComp->GetTeam() != Team)
				{					
					healthComp->RemoveHealth(Damage, Team);
					pHitParticle->ActivateSystem(true);					
				}
					
				break;
			case BulletType::Falloff:
				if (healthComp->GetTeam() != Team)
				{
					healthComp->RemoveHealth(Damage, Team);
					pHitParticle->ActivateSystem(true);
				}
					
				break;
			case BulletType::Instant:
				break;
			case BulletType::Explosive:

				//Explosive Directhit is double damage
				if (healthComp->GetTeam() != Team)
				{
					healthComp->RemoveHealth(Damage, Team);
					//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Magenta, "HIT");
					ExplosionSFX->Play();
					pHitParticle->ActivateSystem(true);
				}							
					
				break;
			default:
				break;
			}



			if (healthComp->GetTeam() != Team)
			{
				ptr = OtherActor->GetComponentByClass(USFXComponent::StaticClass());
				if (ptr != nullptr && Type != BulletType::Explosive)
				{
					auto SFXComp = Cast<USFXComponent>(ptr);
					SFXComp->Play();
				}
			}
	
				if (OtherActor->IsA(APlayerCharacter::StaticClass()))
				{

					auto player = Cast<APlayerCharacter>(OtherActor);
					auto charController = Cast<APlayerController>(player->GetController());
					charController->ClientPlayForceFeedback(player->HitRumble,false,true,"GunRumble");
				}

				if (OtherActor->IsA(ATrainWalls::StaticClass()))
				{
					auto act = GetOwner();
					act = act->GetOwner();
					if(act != nullptr)
					{
						if (act->IsA(APlayerCharacter::StaticClass()))
						{
							//TerrorTrainInstance->GetSoundManager()->Play(HitWallSFX);
							if (healthComp->GetHealth() <= 0.0f)
							{
								auto player = Cast<APlayerCharacter>(act);
								//player->Money->AddMoneyFloat(Cast<APlayerCharacter>(OtherActor)->KllingMoney);	
							}
						}
					}
					
				}
		}
		if (Type == BulletType::Explosive)
		{
			_DoExplosive = true;
		}
		if (ParticleBP != NULL)
		{
			_pParticleActor = GetWorld()->SpawnActor<AActor>(ParticleBP, GetActorLocation(), GetActorRotation(), FActorSpawnParameters());
		}
		Destroy();
	}

}

void ABullet::CheckExplosiveRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (_DoExplosive)
	{
		if (OtherActor != nullptr)
		{			
			auto ptr = OtherActor->GetComponentByClass(UHealthComponent::StaticClass());
			if (ptr != nullptr)
			{
				auto healthComp = Cast<UHealthComponent>(ptr);
				if (healthComp->GetTeam() != Team)
				{
						auto Distance = GetDistanceTo(OtherActor) / 100;
						healthComp->RemoveHealth(Damage/Distance, Team);
				}
			}
			
		}
	}
}

void ABullet::Shoot(FVector Direction)
{	
	_ProjectileMovementComponent->Velocity = _Spread.RotateVector(Direction) * BulletSpeed;
	_ProjectileMovementComponent->UpdateComponentVelocity();

	_Direction = Direction;
}

int ABullet::GetTeam()
{
	return Team;
}

void ABullet::SetTeam(int team)
{
	Team = team;
}

void ABullet::SetSpread(FRotator Spread)
{
	_Spread = Spread;
}

float ABullet::GetDamage()
{
	return Damage;
}

void ABullet::SetDamage(float damage)
{
	Damage = damage;
}

float ABullet::GetSpeed()
{
	return BulletSpeed;
}

void ABullet::SetSpeed(float speed)
{
	BulletSpeed = speed;
}

BulletType ABullet::GetType()
{
	return Type;
}

void ABullet::SetType(BulletType type)
{
	Type = type;
}

