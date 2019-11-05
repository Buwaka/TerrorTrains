// Fill out your copyright notice in the Description page of Project Settings.

#include "DropManager.h"
#include "HealthDrops.h"
#include "AssetRegistry/Public/IAssetRegistry.h"
#include "AssetRegistry/Public/AssetRegistryModule.h"
#include "Runtime/Core/Public/HAL/FileManager.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Engine/Classes/Engine/ObjectLibrary.h"
#include "Engine/Engine.h"
#include "TerrorTrainInstance.h"


bool ListAllBlueprintsInPath(FString Path, TArray<UClass*>& Result, UClass* Class)
{
	auto Library = UObjectLibrary::CreateLibrary(Class, true, GIsEditor);
	//Library->AddToRoot();
	Library->LoadBlueprintAssetDataFromPath(Path);

	TArray<FAssetData> Assets;
	Library->GetAssetDataList(Assets);

	for (auto& Asset : Assets)
	{
		UBlueprint* bp = Cast<UBlueprint>(Asset.GetAsset());
		if (bp)
		{
			auto gc = bp->GeneratedClass;
			if (gc)
			{
				Result.Add(gc);
			}
		}
		else
		{
			auto GeneratedClassName = (Asset.AssetName.ToString() + "_C");

			UClass* Clazz = FindObject<UClass>(Asset.GetPackage(), *GeneratedClassName);
			if (Clazz)
			{
				Result.Add(Clazz);
			}
			else
			{
				UObjectRedirector* RenamedClassRedirector = FindObject<UObjectRedirector>(Asset.GetPackage(), *GeneratedClassName);
				if (RenamedClassRedirector)
				{
					Result.Add(CastChecked<UClass>(RenamedClassRedirector->DestinationObject));
				}
			}

		}
	}

	return true;
}


// Sets default values
ADropManager::ADropManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	//GEEN BACKSLASH OP HET EINDE
	/*DropsFolder = "/Game/Drops";



	TArray<UClass*> files = TArray<UClass*>();
	ListAllBlueprintsInPath(DropsFolder, files, ABaseDrop::StaticClass());

	for (auto BP : files)
	{
		_pBaseDrops.Add(BP->ClassGeneratedBy->GetFName().ToString(), BP);
	}*/

	

}


TArray<ABaseDrop*> ADropManager::SpawnRandomDrops(AActor* context, FVector spawnBox, int amount, FVector offset)
{
	TArray<ABaseDrop*> ret = TArray<ABaseDrop*>();
	TArray<FString> dropArray = TArray<FString>();
	_pBaseDrops.GenerateKeyArray(dropArray);


	for (int i = 0; i < amount; i++)
	{
		float XPos = FMath::FRandRange((-spawnBox.X), spawnBox.X);
		float YPos = FMath::FRandRange((-spawnBox.Y), spawnBox.Y);

		auto VectorPos = FVector(XPos, YPos, spawnBox.Z * 2) + offset;
		//VectorPos += context->GetActorLocation();
		auto rand = FMath::RandRange(0, _pBaseDrops.Num() - 1);
		const FString randomDrop = dropArray[rand];
		//retry when failed to spawn
		auto drop = SpawnDrop(context, VectorPos, randomDrop);
		if (drop == nullptr)
		{
			i--;
		}
		else
		{
			ret.Add(drop);
		}
	}

	return ret;
}

ABaseDrop* ADropManager::SpawnDrop(AActor * context, FVector location, FString type, bool AttachToOwner)
{
	if (_pBaseDrops.Find(type) == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Type not found"));
		return nullptr;
	}


	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Owner = context;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	auto drop = context->GetWorld()->SpawnActor<ABaseDrop>(_pBaseDrops[type], location, FRotator::ZeroRotator, SpawnInfo);

	if (drop == NULL)
	{
		return nullptr;
	}

	if (AttachToOwner && drop != NULL)
	{
		FAttachmentTransformRules aRules{ EAttachmentRule::KeepRelative,true };
		drop->AttachToActor(context, aRules);
	}


	return drop;
}

void ADropManager::DeleteDrops()
{
	for (int i = 0; i < _pDrops.Num(); i++)
	{
		if (_pDrops[i] != nullptr)
		{
			_pDrops[i]->Destroy();			
		}
	}
}

void ADropManager::BeginPlay()
{
	TerrorTrainInstance->SetDropManager(this);
}

void ADropManager::Tick(float DeltaTime)
{
}
