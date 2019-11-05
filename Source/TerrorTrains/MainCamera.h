// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "MainCamera.generated.h"

UCLASS()
class TERRORTRAINS_API AMainCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMainCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/** Camera Component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		class UCameraComponent* CameraComponent;
	UPROPERTY()
		TArray<APlayerCharacter*> myCharactersTeam1;
	UPROPERTY()
		TArray<APlayerCharacter*> myCharactersTeam0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		float MaxDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Settings)
		float MinDistanceChange{5.f};
private:
	UPROPERTY()
		float _PrevLenght = 0;
	UPROPERTY()
		float _PrevChanges = 0;
	UPROPERTY()
		bool _FirstTime;
	UPROPERTY()
		FVector _PrevLocation{};
	UPROPERTY()
		FVector _GoToValue {};
	UPROPERTY()
		float _TimeOfTranslate{0.1f};
};
