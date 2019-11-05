// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
class APlayerCharacter;
#include "GameFramework/Actor.h"
#include "CharacterManager.generated.h"

UCLASS()
class TERRORTRAINS_API ACharacterManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void CreatePlayers();
	UFUNCTION()
	void CheckPlayers();
	UFUNCTION()
	void Respawn(APlayerCharacter*& pPlayerToRespawn);
	UFUNCTION()
	void SetRespawnLocation(FVector respawnLoc,int TeamNum);
	UFUNCTION()
	void SetRespawnRotation(FRotator respawnRot, int TeamNum);
	UFUNCTION(BlueprintCallable, Category = Team)
	TArray<APlayerCharacter*> GetTeam(int team);
	UFUNCTION()
	void Reset();
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Players)
	int _NumPlayersPerTeam{1};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RespawnLocations)
		TArray < FVector> RespawnTeam1{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RespawnLocations)
		TArray < FVector> RespawnTeam2{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RespawnLocations)
	FRotator RespawnRotTeam1 {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RespawnLocations)
	FRotator RespawnRotTeam2 {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
	TArray<TSubclassOf<class APlayerCharacter>> PlayerBP1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Player)
		TArray<TSubclassOf<class APlayerCharacter>> PlayerBP2;
	UPROPERTY(BlueprintReadOnly)
		int TotalDeathsTeam1{};
	UPROPERTY(BlueprintReadOnly)
		int TotalDeathsTeam2{};

	UFUNCTION(BlueprintCallable)
		void GiveMoney(int team, float amount);
	UFUNCTION(BlueprintCallable)
		void RemoveMoney(int team, float amount);	
	UFUNCTION(BlueprintCallable)
		float GetMoney(int team);
	UFUNCTION(BlueprintCallable)
		int GetTeamAmmo(int team);
	UFUNCTION(BlueprintCallable)
		int GetAmmo(int team, int player);
	UFUNCTION(BlueprintCallable)
		APlayerCharacter* GetPlayer(int team, int player);

private:
	UFUNCTION()
	void SpawnMoneyDrop(FVector location, float amount);

	UPROPERTY()
	TArray<APlayerCharacter*> _pTeam1;
	UPROPERTY()
	TArray<APlayerCharacter*> _pTeam2;
	UPROPERTY()
	TArray<AActor*> _pControllers;
	UPROPERTY()
	int PlayerIndex{};
};
