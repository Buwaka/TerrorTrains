// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MoneyComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRORTRAINS_API UMoneyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMoneyComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetMoney();

	UFUNCTION(BlueprintCallable)
	void SetMoney(float value);

	UFUNCTION(BlueprintCallable)
	void AddMoneyFloat(float value);
	UFUNCTION(BlueprintCallable)
	void AddMoney(UMoneyComponent* value);

	UFUNCTION(BlueprintCallable)
	void SubstractMoneyFloat(float value);
	UFUNCTION(BlueprintCallable)
	void SubstractMoney(UMoneyComponent* value);

	UFUNCTION(BlueprintCallable)
	bool CanAffordFloat(float value);
	UFUNCTION(BlueprintCallable)
	bool CanAfford(UMoneyComponent* value);

	UFUNCTION(BlueprintCallable)
		void SetTeamMate(UMoneyComponent* teammate);

	UFUNCTION(BlueprintCallable)
		float GetKillValue();

	UFUNCTION(BlueprintCallable)
		void SetKillValue(float value);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money)
	float DefaultMoney = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Money)
		float DefaultKillMoney = 0;

private:
	UPROPERTY()
	float _Money;
	UPROPERTY()
		float _KillMoney;
	UPROPERTY()
		UMoneyComponent* _pTeammate;
};
