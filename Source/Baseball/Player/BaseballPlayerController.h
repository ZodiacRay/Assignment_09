#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseballPlayerController.generated.h"

UCLASS()
class BASEBALL_API ABaseballPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ABaseballPlayerController();
	
	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void OnRep_PlayerState() override;
	
public:
	UFUNCTION(Client, Reliable)
	void Client_ReceiveMessage(
		const FString &SenderName, 
		const FString &Message,
		const FLinearColor Color = FLinearColor::Gray 
	);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SendMessage(const FString& Message); 
	
public:
	void HUD_UpdateChat(const FString& Sender, const FString& Message, const FLinearColor Color); 
};
