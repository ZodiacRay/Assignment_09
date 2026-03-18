#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "BaseballGameState.generated.h"

class ABaseballPlayerState; 
UCLASS()
class BASEBALL_API ABaseballGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	ABaseballGameState();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_CurrentTurnPlayer();

	UFUNCTION()
	void OnRep_LimitTime();
	
public:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentTurnPlayer, BlueprintReadOnly, Category = "Baseball")
	TObjectPtr<ABaseballPlayerState> CurrentTurnPlayer;

	UPROPERTY(ReplicatedUsing = OnRep_LimitTime, BlueprintReadOnly, Category = "Baseball")
	int32 LimitTime; 
};
