// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerCharacter.h"
#include "RailsTrackGenaration.h"
#include "Door.h"
#include "Train.generated.h"

UCLASS()
class TERRORTRAINS_API ATrain : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
//		UStaticMeshComponent* pMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USkeletalMeshComponent* pAnimationMesh;
	UFUNCTION(BlueprintCallable, Category = Team)
		void SetTeam(int team);
	UFUNCTION(BlueprintCallable, Category = Team)
		int GetTeam();
	UFUNCTION(BlueprintCallable, Category = Wagon)
		void AddWagon();
	UFUNCTION(BlueprintCallable, Category = Wagon)
		void SetMaxWagon(int maxwagon);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		int GetMaxWagon();
	UFUNCTION(BlueprintCallable, Category = Wagon)
		int GetNumOfWagons();
	UFUNCTION(Category = Wagon)
		void DeleteWagon();
	UFUNCTION(BlueprintCallable)
		void StopTrain(bool stop = false);
	UFUNCTION(BlueprintCallable, Category = Rails)
		void DoRailsBreaking();
	UFUNCTION(BlueprintCallable, Category = Rails)
		void DoRailsNotBreaking();
	UFUNCTION(BlueprintCallable)
		bool GetIsStopped();
	UFUNCTION(BlueprintCallable)
		void StartBreaking(bool breaking = true);
	UFUNCTION(BlueprintCallable)
		bool GetIsBreaking();
	UFUNCTION(BlueprintCallable, Category = Wagon)
		void MoveOut(bool start , FVector dir, bool moveOut);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		bool GetDoneMoveOut();
	UFUNCTION(BlueprintCallable, Category = "Wagon|Door")
		bool BuyWagon(APlayerCharacter* player);
	UFUNCTION(BlueprintCallable, Category = "Wagon|Door")
		void CanAfford(float money);
	UFUNCTION(BlueprintCallable, Category = "Wagon|Door")
		bool AllowedToBuy();
	UFUNCTION(BlueprintCallable, Category = "Wagon|Door")
		int GetNumOfDoorsLeft();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Wagon)
		TArray <TSubclassOf<class ATrainWagon>> WagonBP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Rails)
		TSubclassOf<class ARailsTrackGenaration> RailsBP;
	UFUNCTION(BlueprintCallable, Category = Health)
		void SetHealth(float health);
	UFUNCTION(BlueprintCallable, Category = Wagon)
		ATrainWagon* GetActiveWagon();
	UFUNCTION(BlueprintCallable, Category = Rails)
		ARailsTrackGenaration* GetTrackGenerator();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FVector LocationOffset {500, 0, 0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wagon|Door")
		float StartDoorCost{200};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wagon|Door")
		float CostIncrease{50};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Door)
		TArray <ADoor*> DoorArr{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		UAnimSequence* pAnim {};

private:
	UPROPERTY()
		TArray<ATrainWagon*> _pWagons;
	UPROPERTY()
		ATrainWagon* _pActiveWagon;
	UPROPERTY()
		float _Time;
	UPROPERTY()
		FVector _TrainLocation;
	UPROPERTY()
		FVector _OGTrainLocation;
	UPROPERTY()
		FVector _LastTrainLocation;
	UPROPERTY()
		FVector _GoToLocation{};
	UPROPERTY()
		int _MaxNumberOfWagons = 3;
	UPROPERTY()
		int _WagonsAlive = 0;
	UPROPERTY()
		int _DoorsLeft = 0;
	UPROPERTY()
		float _Speed = 3.f;
	UPROPERTY()
		float _Offset = 200.f;
	UPROPERTY()
		FVector _MeshBounds;
	UPROPERTY()
		bool _Stop;
	UPROPERTY()
		bool _MoveOut;
	UPROPERTY()
		FVector _MoveDir;
	UPROPERTY()
		float _TimeOfTranslate{};
	UPROPERTY()
		bool _DoneTranslate{};
	UPROPERTY()
		bool _GoBack{};
	UPROPERTY()
	bool _FirstTime = true;
	UPROPERTY()
	bool _SwapDir{};
	UPROPERTY()
		int _Team;
	UPROPERTY()
	ARailsTrackGenaration* _Rails;
	UPROPERTY()
		TArray<int> _UsedWagons;
	UPROPERTY()
		ADoor* pChest;
	UPROPERTY()
		bool _Breaking{false};
	UPROPERTY()
		bool _PrevStop{ false };
};
