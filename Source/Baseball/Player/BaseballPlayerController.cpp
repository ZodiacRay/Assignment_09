#include "BaseballPlayerController.h"
#include "Baseball/Game/BaseballGameMode.h"
#include "Baseball/UI/BaseballHUD.h"

ABaseballPlayerController::ABaseballPlayerController()
{
	bReplicates = true;
}

void ABaseballPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	if (IsLocalController())
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
		bShowMouseCursor = true;
	}
}

void ABaseballPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ABaseballPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}


void ABaseballPlayerController::Client_ReceiveMessage_Implementation(const FString& SenderName, const FString& Message,
                                                                     const FLinearColor Color)
{
	HUD_UpdateChat(SenderName, Message, Color);
}

bool ABaseballPlayerController::Server_SendMessage_Validate(const FString& Message)
{
	if (Message.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] Message is Empty"));
		return false; 
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] Message is valid")); // 여긴 나옴 
		return true; 
	}
	
}

void ABaseballPlayerController::Server_SendMessage_Implementation(const FString& Message)
{
	AGameModeBase* RawGM = GetWorld()->GetAuthGameMode();
	if (!RawGM)
	{
		UE_LOG(LogTemp, Error, TEXT("[Server] AuthGameMode is NULL!")); // 여기 나옴 
		return;
	}
	
	ABaseballGameMode* GM = Cast<ABaseballGameMode>(RawGM);
	if (GM)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] GameMode Cast Success. Calling ReceiveGuess..."));
		GM->ReceiveGuess(this, Message);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("[Server] GameMode Cast Failed! Check your GameMode Class."));
	}
}

void ABaseballPlayerController::HUD_UpdateChat(const FString& Sender, const FString& Message, const FLinearColor Color)
{
	if (ABaseballHUD* BaseballHUD = GetHUD<ABaseballHUD>())
	{
		FString CombinedMessage = FString::Printf(TEXT("%s : %s"), *Sender, *Message); 
		BaseballHUD->UpdateChat(CombinedMessage, Color);
	}
}


