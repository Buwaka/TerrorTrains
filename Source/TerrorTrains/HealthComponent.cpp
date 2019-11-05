// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthComponent.h"
#include "Engine/Engine.h"
#include "PlayerCharacter.h"
#include "MoneyComponent.h"
#include "TerrorTrainInstance.h"
#include "CharacterManager.h"

#define Max(x,y) ((x>y) ? x : y)

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ResetHealth();
	_Hit = false;
	_Timer = 0;
	_InvincibilityTime = DefaultInvincibilityTime;
	_Team = DefaultTeam;
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(_Hit)
	{
		_Timer += DeltaTime;
		if(_Timer > _InvincibilityTime)
		{
			_Hit = false;
			_Timer = 0;
			_InvincibilityTime = DefaultInvincibilityTime;
		}
	}
}

void UHealthComponent::Initialize(int team, float health, float invincibilityTime)
{
	ResetHealth(health);
	_Hit = false;
	_Timer = 0;
	_InvincibilityTime = invincibilityTime;
	_Team = team;
}

//Add health, heals, med kits,...
void UHealthComponent::AddHealth(float health)
{
	_Health += health;
}

//remove health, by taking damage for example (Health Component does not handle death mechanics!)
void UHealthComponent::RemoveHealth(float health, int team, bool TriggerInvincible)
{
	if(!_Hit)
	{
		_Health -= health;
		_Hit = TriggerInvincible;
		auto owner = GetOwner();
		if (owner)
		{
			if (owner->IsA(APlayerCharacter::StaticClass()))
			{
				if (ParticleBP != NULL)
				{
					GetWorld()->SpawnActor<AActor>(ParticleBP, owner->GetActorLocation(), owner->GetActorRotation());
				}
			}
			//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Green, FString::Printf(TEXT("Health: %f"), _Health));


			if (_Health <= 0.f)
			{
				auto comp = owner->GetComponentByClass(UMoneyComponent::StaticClass());
				float  money{};
				if (comp)
				{
					auto moneycomp = Cast<UMoneyComponent>(comp);
					money = moneycomp->GetKillValue();
					TerrorTrainInstanceFromComponent->GetCharacterManager()->GiveMoney(team, money);
				}
			}
		}

	}
}

//Get current health
float UHealthComponent::GetHealth()
{
	return _Health;
}

//Get amount of health actor has above its default value, by upgrades or boosts or whatnot
float UHealthComponent::GetOverHealth()
{
	return Max(_Health - DefaultHealth, 0);
}

//Set health back to the default value
void UHealthComponent::ResetHealth(float health)
{
	if (health == -1.f)
	{
		_Health = DefaultHealth;
	}
	else 
	{
		_Health = health;
	}
}

//make actor invincible for [duration] amount of time
void UHealthComponent::SetInvincible(float duration)
{
	_InvincibilityTime = duration;
	_Hit = true;
}

int UHealthComponent::GetTeam()
{
	return _Team;
}

void UHealthComponent::SetTeam(int team)
{
	_Team = team;
}

