// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAICPP.h"

// Sets default values
ABaseAICPP::ABaseAICPP()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseAICPP::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseAICPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseAICPP::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

UAISenseConfig* ABaseAICPP::GetPerceptionSenseConfig(AAIController* Controller, TSubclassOf<UAISense> SenseClass)
{
	UAISenseConfig * result = nullptr;

	FAISenseID Id = UAISense::GetSenseID(SenseClass);
	if (!Id.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GetPerceptionSenseConfig: Wrong Sense ID"));
	}
	else if (Controller == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetPerceptionSenseConfig: Controller == nullptr"));
	}
	else
	{
		UAIPerceptionComponent *Perception = Controller->GetAIPerceptionComponent();
		if (Perception == nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("GetPerceptionSenseConfig: Perception == nullptr"));
		}
		else
		{
			result = Perception->GetSenseConfig(Id);
		}
	}

	return result;
}

bool ABaseAICPP::SetSightRange(AAIController* Controller, float SightRange)
{
	UAISenseConfig* Config = GetPerceptionSenseConfig(Controller, UAISense_Sight::StaticClass());
	if (Config == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SetSightRange: Config == nullptr"));
		return false;
	}
	else
	{
		UAISenseConfig_Sight *ConfigSight = Cast<UAISenseConfig_Sight>(Config);

		UE_LOG(LogTemp, Verbose, TEXT("SetSightRange was %f %f, setting to %f %f")
			, ConfigSight->SightRadius, ConfigSight->LoseSightRadius, SightRange, (ConfigSight->LoseSightRadius - ConfigSight->SightRadius + SightRange));

		// Save original lose range
		float LoseRange = ConfigSight->LoseSightRadius - ConfigSight->SightRadius;
		ConfigSight->SightRadius = SightRange;
		// Apply lose range to new radius of the sight
		ConfigSight->LoseSightRadius = ConfigSight->SightRadius + LoseRange;
		UAIPerceptionComponent *Perception = Controller->GetAIPerceptionComponent();
		Perception->RequestStimuliListenerUpdate();
	}

	return true;
}