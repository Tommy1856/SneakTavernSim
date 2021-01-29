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

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable, ClassGroup = HeadMountedDisplay)
class SNEAKTAVERNSIM_API UInventoryComponentCPP : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<FString> ItemsInInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FVector DropLocal;

	static float CurrentWeightCPP;

	// Sets default values for this component's properties
	UInventoryComponentCPP();

	UFUNCTION(BlueprintPure)
		static FSplitStringReturn SplitString(FString StringToSplit, FString FirstSplit, FString SecondSplit);

	UFUNCTION(BluePrintCallable)
		static bool CheckIfEnoughWeight(FString ItemInfo, float ContainersCurrentWeight, float ContainerMaxWeight, float& NewWeight, FString& NewItemtoAddInfo, FString& OverflowingItemInfo);

	UFUNCTION(BluePrintCallable)
		static void CheckIfStackable(TArray<FString> Items, FString ItemToMerge, TArray<FString>& NewItems, FString& Overflow);

	UFUNCTION(BluePrintCallable)
		static bool MergeSlots(FString SlotToMergeTo, FString ItemToMerge, FString& NewItemInfo, FString& OverFlowingItemInfo);

	UFUNCTION(BluePrintCallable)
		static bool CheckIfEnoughItems(TArray<FString> RequiredItems, TArray<FString> ItemsInArray, int InQuanity, TArray<FString>& NewItems, float& WeightToRemove, TArray<FString>& ItemsToAddToStructure, TArray<FString>& NewRequiredItemsFoStructure, bool isStructure, bool SkipRemovingItems, TArray<bool>& ItemsDontHave);

	UFUNCTION(BluePrintPure)
		static TArray<int> SpawnItems(float MaxItemSpawnChance, TArray<float> SpawnChances, int HowManyItems, int Seed);

	UFUNCTION(BluePrintCallable)
		static TArray<FString> GetAmmo(FString AmmoName, FString TypeToCheck, TArray<FString> Inventory, bool& Success);

	UFUNCTION(BluePrintCallable)
		static FString ReloadMagazine(FString MagazineInfo, FString AmmoToAdd);

	UFUNCTION(BluePrintCallable)
		static FString UpdateQuanity(FString Item, int NewQuanity);
	/*
	UFUNCTION(BluePrintCallable)
		static int CanReload(TArray<FString> Inventory, int MaxAmountToReload, FString AmmoNameToReload, TArray<FString> &NewItems);
		*/
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

};


