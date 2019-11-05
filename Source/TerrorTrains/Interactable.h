// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TerrorTrains/PlayerCharacter.h"
#include "Interactable.generated.h"

//DECLARE_DYNAMIC_DELEGATE_OneParam(FInteractable, UObject*, player);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRORTRAINS_API UInteractable : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractable();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Interact(APlayerCharacter* player);


	//UPROPERTY()
	//FInteractable* InteractFunction;

private:


		
};
