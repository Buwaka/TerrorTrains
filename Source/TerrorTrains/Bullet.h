// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Bullet.generated.h"

UENUM(BlueprintType)
enum class BulletType : uint8
{
	Simple UMETA(DisplayName = "Simple"), 
	Falloff UMETA(DisplayName = "Falloff"),
	Instant UMETA(DisplayName = "Instant"),
	Explosive UMETA(DisplayName = "Explosive")
};

class USFXComponent;
UCLASS(Blueprintable)
class TERRORTRAINS_API ABullet : public AActor
{
	GENERATED_BODY()


	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Functions
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void CheckExplosiveRadius(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void Shoot(FVector Direction);

	UFUNCTION(BlueprintCallable)
	int GetTeam();

	UFUNCTION(BlueprintCallable)
	void SetTeam(int team);

	UFUNCTION(BlueprintCallable)
	void SetSpread(FRotator Spread);

	UFUNCTION(BlueprintCallable)
	float GetDamage();

	UFUNCTION(BlueprintCallable)
	void SetDamage(float damage);

	UFUNCTION(BlueprintCallable)
		float GetSpeed();

	UFUNCTION(BlueprintCallable)
		void SetSpeed(float speed);

	UFUNCTION(BlueprintCallable)
	BulletType GetType();

	UFUNCTION(BlueprintCallable)
	void SetType(BulletType type);


	//Variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		UStaticMeshComponent *pBulletMesh;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
	//	USkeletalMeshComponent *pBulletSkeletalMesh;

	//gets called when collision occurs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Particles")
		UParticleSystemComponent *pHitParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Particles")
		TSubclassOf<AActor> ParticleBP;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet|Sphere")
		USphereComponent * pExplosiveRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		int Team = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		float BulletSpeed = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		float LifeTime = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet")
		BulletType Type = BulletType::Simple;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Falloff")
		float FalloffDeceleration = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Explosive")
		float Radius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bullet | Explosive", meta = (ClampMin = 0.f, UIMin = 0.f, ClampMax = 1.0f, UIMax = 1.0f))
		float OuterRadiusDamagePercentage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SFX")
		USFXComponent* ExplosionSFX;

private:
	UPROPERTY()
	UProjectileMovementComponent* _ProjectileMovementComponent;

	UPROPERTY()
	float _Timer{};

	UPROPERTY()
	bool _DoExplosive{};

	UPROPERTY()
	AActor* _pMainActor{};

	UPROPERTY()
	AActor* _pParticleActor {};

	UPROPERTY()
	FRotator _Spread{};

	UPROPERTY()
	FVector _Direction{};


};
