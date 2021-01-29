// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/AIPerceptionComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseAICPP.generated.h"

UCLASS()
class SNEAKTAVERNSIM_API ABaseAICPP : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICPP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	/*
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	static UAISenseConfig* GetPerceptionSenseConfig(AAIController *Controller, TSubclassOf<UAISense> SenseClass);

	*/
};


