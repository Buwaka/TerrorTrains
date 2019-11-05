// Fill out your copyright notice in the Description page of Project Settings.

#include "MoneyComponent.h"
#include <algorithm>
#include "Engine/Engine.h"

// Sets default values for this component's properties
UMoneyComponent::UMoneyComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMoneyComponent::BeginPlay()
{
	Super::BeginPlay();

	_Money = DefaultMoney;
	_KillMoney = DefaultKillMoney;
}


// Called every frame
void UMoneyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UMoneyComponent::GetMoney()
{
	return _Money;
}

void UMoneyComponent::SetMoney(float value)
{
	_Money = value;
}

void UMoneyComponent::AddMoneyFloat(float value)
{
	_Money += value;
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Money: %f"), _Money));
}

void UMoneyComponent::AddMoney(UMoneyComponent * value)
{
	_Money += value->GetMoney();
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Money: %f"), _Money));
}

void UMoneyComponent::SubstractMoneyFloat(float value)
{
	if (_pTeammate != nullptr)
	{
		if (value <= _Money)
		{
			_Money = _Money - value;
		}
		else
		{
			_Money = abs(_Money - value);
			_pTeammate->SetMoney(_pTeammate->GetMoney() - _Money);
			_Money = 0;
		}
	}
	else
	{
		_Money = std::min(_Money - value, 0.0f);
	}
}

void UMoneyComponent::SubstractMoney(UMoneyComponent * value)
{
	if (_pTeammate != nullptr)
	{
		if (value->GetMoney() <= _Money)
		{
			_Money = _Money - value->GetMoney();
		}
		else
		{
			_Money = abs(_Money - value->GetMoney());
			_pTeammate->SetMoney(_pTeammate->GetMoney() - _Money);
			_Money = 0;
		}
	}
	else
	{
		_Money = _Money - value->GetMoney();
	}
	//GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Orange, FString::Printf(TEXT("Money: %f"), _Money));
}

bool UMoneyComponent::CanAffordFloat(float value)
{
	if (_pTeammate != nullptr)
	{
		return value <= (_Money + _pTeammate->GetMoney());
	}
	else { return value <= _Money; }
}

bool UMoneyComponent::CanAfford(UMoneyComponent* value)
{
	if (_pTeammate != nullptr)
	{
		return value->GetMoney() <= (_Money + _pTeammate->GetMoney());
	}
	else { return value->GetMoney() <= _Money; }

}

void UMoneyComponent::SetTeamMate(UMoneyComponent* teammate)
{
	_pTeammate = teammate;
}

float UMoneyComponent::GetKillValue()
{
	return _KillMoney;
}

void UMoneyComponent::SetKillValue(float value)
{
	_KillMoney = value;
}

