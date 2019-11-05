// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseDrop.h"
#include "DropManager.generated.h"

UENUM(BlueprintType)
enum class DropType : uint8
{
	Health UMETA(DisplayName = "Health"),
};


UCLASS()
class TERRORTRAINS_API ADropManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADropManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropsType)
		TSubclassOf<class AHealthDrops> HealthBP;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropsType)
		TMap<FString, TSubclassOf<class ABaseDrop>> _pBaseDrops;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Drops)
		FString DropsFolder;

	UFUNCTION()
		TArray<ABaseDrop*> SpawnRandomDrops(AActor * context, FVector spawnBox, int amount, FVector offset);
	UFUNCTION()
		ABaseDrop* SpawnDrop(AActor* context, FVector location, FString type, bool AttachToOwner = true);
	UFUNCTION()
		void DeleteDrops();
private:
	UPROPERTY()
		TArray<ABaseDrop*> _pDrops;
	/*UPROPERTY()
		TMap<FString,TSubclassOf<class ABaseDrop>> _pBaseDrops;*/
	UPROPERTY()
		int _UniqueDropCount;
};
