// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponentCPP.h"


//Used to extract information from InventoryInfo
FSplitStringReturn UInventoryComponentCPP::SplitString(FString StringToSplit, FString FirstSplit, FString SecondSplit) {
	FSplitStringReturn ReturnString;
	FString FirstSplitRight;
	StringToSplit.Split(FirstSplit, &ReturnString.Left, &FirstSplitRight);
	FirstSplitRight.Split(SecondSplit, &ReturnString.Left, &ReturnString.Right);
	return ReturnString;
}

//Checks if enough space in inventory
bool UInventoryComponentCPP::CheckIfEnoughWeight(FString ItemInfo, float ContainersCurrentWeight, float ContainerMaxWeight, float& NewWeight, FString& NewItemtoAddInfo, FString& OverflowingItemInfo) {
	int OverflowQunaity = FCString::Atoi(*SplitString(ItemInfo, "Quanity=", ",").Left) - ((ContainerMaxWeight - ContainersCurrentWeight) / FCString::Atof(*SplitString(ItemInfo, "Weight=", ",").Left));
	if (OverflowQunaity <= 0) {
		NewItemtoAddInfo = ItemInfo;
		NewWeight = ContainersCurrentWeight + (FCString::Atof(*SplitString(ItemInfo, "Weight=", ",").Left) * FCString::Atoi(*SplitString(ItemInfo, "Quanity=", ",").Left));
		return true;
	}
	else {
		FString Left;
		FString Right;
		ItemInfo.Split("Quanity=", &Left, &Right);
		Right.Split(",", NULL, &Right);
		if (FCString::Atoi(*SplitString(ItemInfo, "Quanity=", ",").Left) - OverflowQunaity > 0) {
			NewItemtoAddInfo = (Left + "Quanity=" + FString::FromInt(FCString::Atoi(*SplitString(ItemInfo, "Quanity=", ",").Left) - OverflowQunaity) + "," + Right);
		}
		OverflowingItemInfo = (Left + "Quanity=" + FString::FromInt(OverflowQunaity) + "," + Right);
		NewWeight = ContainersCurrentWeight + ((FCString::Atof(*SplitString(ItemInfo, "Weight=", ",").Left) * (FCString::Atof(*SplitString(ItemInfo, "Quanity=", ",").Left))) - (OverflowQunaity * (FCString::Atof(*SplitString(ItemInfo, "Weight=", ",").Left))));
		return false;

	}
}


//Used when an item is added to the inventory (NOT when a slot is dragged) e.g when an item is picked up off the floor
void UInventoryComponentCPP::CheckIfStackable(TArray<FString> Items, FString ItemToMerge, TArray<FString>& NewItems)
{
	NewItems = Items;
	int QuanityToAdd = FCString::Atoi(*SplitString(ItemToMerge, "Quanity=", ",").Left);

	for (int i = 0; Items.Num() > i; i++) {
		//Check if enogu room
		if (QuanityToAdd <= 0) {
			break;
		}
		else {
			if (SplitString(Items[i], "Name=", ",").Left == SplitString(ItemToMerge, "Name=", ",").Left) {
				int OverflowAmount = QuanityToAdd - (FCString::Atoi(*SplitString(Items[i], "MaxStackSize=", ",").Left) - FCString::Atoi(*SplitString(Items[i], "Quanity=", ",").Left));
				if (OverflowAmount <= 0) {
					FString Left;
					FString Right;
					NewItems.RemoveAt(i);
					Items[i].Split("Quanity=", &Left, &Right);
					Right.Split(",", NULL, &Right);
					FString Return = Left + "Quanity=" + FString::FromInt(FCString::Atoi(*SplitString(Items[i], "Quanity=", ",").Left) + QuanityToAdd) + "," + Right;
					NewItems.Insert(Return, i);
					QuanityToAdd = 0;
					break;

				}
				else
				{
					FString Left;
					FString Right;
					QuanityToAdd -= FCString::Atoi(*SplitString(Items[i], "MaxStackSize=", ",").Left) - FCString::Atoi(*SplitString(Items[i], "Quanity=", ",").Left);
					Items[i].Split("Quanity=", &Left, &Right);
					Right.Split(",", NULL, &Right);
					NewItems.RemoveAt(i);
					NewItems.Insert(Left + "Quanity=" + SplitString(Items[i], "MaxStackSize=", ",").Left + "," + Right, i);
				}
			}
		}
	}
	if (Items.Num() == 0) {
		FString Left;
		FString Right;
		ItemToMerge.Split("SlotNum=", &Left, &Right);
		Right.Split(",", NULL, &Right);
		FString Return = Left + Right;
		if (Return == "") {
			NewItems.Add(ItemToMerge);
		}
		else {
			NewItems.Add(Return);
		}
	}
	else if (QuanityToAdd > 0) {
		FString Left;
		FString Right;
		ItemToMerge.Split("SlotNum=", &Left, &Right);
		Right.Split(",", NULL, &Right);
		FString Return = Left + Right;
		if (Return == "") {
			ItemToMerge.Split("Quanity=", &Left, &Right);
			Right.Split(",", NULL, &Right);
			Return = Left + "Quanity=" + FString::FromInt(QuanityToAdd) + "," + Right;
			NewItems.Add(Return);
		}
		else {
			Return.Split("Quanity=", &Left, &Right);
			Right.Split(",", NULL, &Right);
			Return = Left + "Quanity=" + FString::FromInt(QuanityToAdd) + "," + Right;
			NewItems.Add(Return);
		}
	}
}

//Used when the player drags one slot on to another
bool UInventoryComponentCPP::MergeSlots(FString SlotToMergeTo, FString ItemToMerge, FString& NewItemInfo, FString& OverFlowingItemInfo)
{
	//IF SLOT DROPPED ON SLOT, CHECK IF THE SLOT DROPPED CAN BE PUT IN INVENTORY THEN MERGE THE SLOTS
	int OverflowAmount = FCString::Atoi(*SplitString(ItemToMerge, "Quanity=", ",").Left) - (FCString::Atoi(*SplitString(SlotToMergeTo, "MaxStackSize=", ",").Left) - FCString::Atoi(*SplitString(SlotToMergeTo, "Quanity=", ",").Left));
	if (OverflowAmount <= 0) {
		FString Left;
		FString Right;
		SlotToMergeTo.Split("Quanity=", &Left, &Right);
		Right.Split(",", NULL, &Right);
		NewItemInfo = Left + "Quanity=" + FString::FromInt(FCString::Atoi(*SplitString(SlotToMergeTo, "Quanity=", ",").Left) + FCString::Atoi(*SplitString(ItemToMerge, "Quanity=", ",").Left)) + "," + Right;
		return false;
	}
	else
	{
		FString Left;
		FString Right;
		SlotToMergeTo.Split("Quanity=", &Left, &Right);
		Right.Split(",", NULL, &Right);
		NewItemInfo = Left + "Quanity=" + SplitString(SlotToMergeTo, "MaxStackSize=", ",").Left + Right;
		OverFlowingItemInfo = Left + "Quanity=" + FString::FromInt(OverflowAmount) + "," + Right;
		return true;
	}
}


bool UInventoryComponentCPP::CheckIfEnoughItems(TArray<FString> RequiredItems, TArray<FString> ItemsInArray, int InQuanity, TArray<FString>& NewItems, float& WeightToRemove)
{
	TArray<int> RequiredItemsCheck;
	int EnoughItemsCount = 0;
	NewItems = ItemsInArray;
	if (ItemsInArray.Num() >= RequiredItems.Num()) {//check if there are enough items in the inventory to craft the item
		for (int i = 0; i < RequiredItems.Num(); i++) {//need to check each item for each required item
			int QuanityTemp = FCString::Atoi(*SplitString(RequiredItems[i], "Quanity=", ",").Left) * InQuanity; //get the needed quanitiy of the required item
			for (int y = 0; y < ItemsInArray.Num(); y++) {
				if (SplitString(ItemsInArray[y], "Name=", ",").Left == SplitString(RequiredItems[i], "Name=", ",").Left) {//if the item in the inventory is the same item as the required item
					QuanityTemp -= FCString::Atoi(*SplitString(ItemsInArray[y], "Quanity=", ",").Left);//find out if the current item has enough quanity if not then check if there are more of the same items that have enough quanity when added together
					if (QuanityTemp <= 0) {
						RequiredItemsCheck.Add(y);
						EnoughItemsCount += 1;//used to check if the player has enough items to craft
						break;
					}
					else {
						RequiredItemsCheck.Add(y);
					}
				}
			}
		}
		if (EnoughItemsCount == RequiredItems.Num()) {//this is where the items are removed from the inventory
			FString CurrentItemName = "null";
			int QuanityNeeded;
			for (int i = 0; i < RequiredItems.Num(); i++) {
				for (int j = RequiredItemsCheck.Num() - 1; j >= 0; j--) {
					if (SplitString(ItemsInArray[RequiredItemsCheck[j]], "Name=", ",").Left == SplitString(RequiredItems[i], "Name=", ",").Left && SplitString(RequiredItems[i], "IsTool=", ",").Left == "true") {//Check if the current required item is a tool
						float NewDurability = FCString::Atof(*SplitString(ItemsInArray[RequiredItemsCheck[j]], "Durability=", ",").Left) - (FCString::Atof(*SplitString(RequiredItems[i], "DurabilityDamage=", ",").Left) * InQuanity);
						if (NewDurability <= 0) {
							WeightToRemove += FCString::Atof(*SplitString(ItemsInArray[RequiredItemsCheck[j]], "Weight=", ",").Left) * FCString::Atof(*SplitString(RequiredItems[i], "Quanity=", ",").Left);
							NewItems[RequiredItemsCheck[j]] = "";//this index will get removed in using the open inventory function since it checks if array elements are empty *this cannot be removed here as it crashes the engine
						}
						else {
							FString Right;
							FString Left;
							NewItems[RequiredItemsCheck[j]].Split("Durability=", &Left, &Right);
							Right.Split(",", NULL, &Right);
							FString Return = Left + "Durability=" + FString::SanitizeFloat(NewDurability) + "," + Right;
							NewItems[RequiredItemsCheck[j]] = Return;
						}
					}
					else {
						if (SplitString(ItemsInArray[RequiredItemsCheck[j]], "Name=", ",").Left == SplitString(RequiredItems[i], "Name=", ",").Left) {
							if (CurrentItemName == SplitString(ItemsInArray[RequiredItemsCheck[j]], "Name", ",").Left) {}//check if the same item is being removed, if it is then do nothing
							else {//if it is a different item then update the CurrentItemName and update QuanityNeeded - This is done since there could be multiple stacks of items that need to be updated
								CurrentItemName = SplitString(ItemsInArray[RequiredItemsCheck[j]], "Name", ",").Left;
								QuanityNeeded = FCString::Atoi(*SplitString(RequiredItems[i], "Quanity=", ",").Left) * InQuanity;
							}

							int ItemQunaityLeft = (FCString::Atoi(*SplitString(ItemsInArray[RequiredItemsCheck[j]], "Quanity=", ",").Left) - QuanityNeeded);//get how many items are left in the 
							QuanityNeeded -= FCString::Atoi(*SplitString(ItemsInArray[RequiredItemsCheck[j]], "Quanity=", ",").Left);

							if (ItemQunaityLeft <= 0) {
								WeightToRemove += FCString::Atof(*SplitString(ItemsInArray[RequiredItemsCheck[j]], "Weight=", ",").Left) * FCString::Atof(*SplitString(RequiredItems[i], "Quanity=", ",").Left);
								NewItems[RequiredItemsCheck[j]] = "";
							}
							else {
								WeightToRemove += FCString::Atof(*SplitString(ItemsInArray[RequiredItemsCheck[j]], "Weight=", ",").Left) * FCString::Atoi(*SplitString(RequiredItems[i], "Quanity=", ",").Left);
								FString Left;
								FString Right;
								NewItems[RequiredItemsCheck[j]].Split("Quanity=", &Left, &Right);
								Right.Split(",", NULL, &Right);
								NewItems[RequiredItemsCheck[j]] = Left + "Quanity=" + FString::FromInt(ItemQunaityLeft) + "," + Right;
							}
						}
					}
				}
			}
			return true;
		}
		else {
			return false;
		}
	}
	else {
		return false;
	}
}

TArray<int> UInventoryComponentCPP::SpawnItems(float MaxItemSpawnChance, TArray<float> SpawnChances, int HowManyItems) {
	float random;
	TArray<int> ItemsInContainer;
	for (int i = 1; i < (HowManyItems + 1); i++) {
		random = FMath::RandRange(0.0f, MaxItemSpawnChance);
		for (int index = 0; index < SpawnChances.Num(); index++) {
			if (random < SpawnChances[index]) {
				ItemsInContainer.Add(index);
				index = 100;
			}
			else
				random -= SpawnChances[index];
		}
	}
	return ItemsInContainer;
}