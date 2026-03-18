#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseballGameMode.generated.h"

class ABaseballPlayerState;
class ABaseballPlayerController;
UCLASS()
class BASEBALL_API ABaseballGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ABaseballGameMode();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	
public:
	void SendMessageToClient(
		ABaseballPlayerController* ReceivePlayer, 
		const FString& SenderID,
		const FString& Message, 
		FLinearColor Color
	);
	
	void BroadcastMessage(
		const FString& SenderID,
		const FString& Message,
		FLinearColor Color
	);

protected:
	void GenerateSecretNumber(); 
	
	bool IsValidGuessNumber(const FString& GuessNumber);
	
	bool JudgeGuessNumber(const FString& GuessNumber);
	
	void StartTimer(); 
	
	void OnTimeOut(); 
	
	void SynchronizeTimer();
	
	void SwitchTurn(); 
	
public:
	void ReceiveGuess(ABaseballPlayerController* Sender, const FString& GuessNumber);
	
	void ResetGame(); 
	
private:
	UPROPERTY()
	FString SecretNumber; 
	
	UPROPERTY()
	TArray<TObjectPtr<ABaseballPlayerController>> PlayerList;
	
	UPROPERTY()
	int32 CurrentTurnIndex; 
	
	UPROPERTY()
	TObjectPtr<ABaseballPlayerState> CurrentTurnPlayerState;
	
	FTimerHandle TurnTimerHandle;
	
	FTimerHandle SyncTimerHandle; 
};
