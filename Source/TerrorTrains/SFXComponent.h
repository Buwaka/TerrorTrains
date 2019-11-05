// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SFXComponent.generated.h"

class USoundWave;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TERRORTRAINS_API USFXComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USFXComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void Play();

	UFUNCTION(BlueprintCallable)
	void Pause();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		USoundWave* SFX;

	//range [0,1]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		float Volume = 1.0f;

private:
	UPROPERTY()
		float _Volume;
		
};
