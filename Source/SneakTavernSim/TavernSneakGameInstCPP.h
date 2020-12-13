// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TavernSneakGameInstCPP.generated.h"

/**
 * 
 */
UCLASS()
class SNEAKTAVERNSIM_API UTavernSneakGameInstCPP : public UGameInstance
{
	GENERATED_BODY()
		UFUNCTION(BlueprintPure, Category = "VictoryBPLibrary|File IO")
		static bool GetSaveGameFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext);
};
