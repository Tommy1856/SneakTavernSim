// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/Canvas.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponentCPP.generated.h"

USTRUCT(BlueprintType)
struct FSplitStringReturn
{
	GENERATED_USTRUCT_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Right;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, ClassGroup = HeadMountedDisplay)
class SNEAKTAVERNSIM_API UInventoryComponentCPP : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> ItemsInInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DropLocal;

	static float CurrentWeightCPP;

	UFUNCTION(BlueprintPure)
		static FSplitStringReturn SplitString(FString StringToSplit, FString FirstSplit, FString SecondSplit);

	UFUNCTION(BluePrintCallable)
		static bool CheckIfEnoughWeight(FString ItemInfo, float ContainersCurrentWeight, float ContainerMaxWeight, float& NewWeight, FString& NewItemtoAddInfo, FString& OverflowingItemInfo);

	UFUNCTION(BluePrintCallable)
		static void CheckIfStackable(TArray<FString> Items, FString ItemToMerge, TArray<FString>& NewItems);

	UFUNCTION(BluePrintCallable)
		static bool MergeSlots(FString SlotToMergeTo, FString ItemToMerge, FString& NewItemInfo, FString& OverFlowingItemInfo);

	UFUNCTION(BluePrintCallable)
		static bool CheckIfEnoughItems(TArray<FString> RequiredItems, TArray<FString> ItemsInArray, int InQuanity, TArray<FString>& NewItems, float& WeightToRemove);

	UFUNCTION(BluePrintPure)
		static TArray<int> SpawnItems(float MaxItemSpawnChance, TArray<float> SpawnChances, int HowManyItems);

		
};
