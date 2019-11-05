// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HealthComponent.h"
#include "MoneyVault.generated.h"

UCLASS()
class TERRORTRAINS_API AMoneyVault : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoneyVault();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropMoney)
		float MoneyPerDrop{50.f};
	UFUNCTION()
		void DropMoney(float Damage);
	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			FVector NormalImpulse, const FHitResult& Hit);
	UPROPERTY()
		int Team{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DropLocation)
		FVector DropLocation{0.f,100.f,0.f};
private:
	
	
};
