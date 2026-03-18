// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "BaseballPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BASEBALL_API ABaseballPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ABaseballPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override; 
	
public:
	UPROPERTY(Replicated)
	FString PlayerName;
	
	UPROPERTY()
	int32 MaxLeftChance = 3; 
	
	UPROPERTY(Replicated)
	int32 LeftChance;
};
