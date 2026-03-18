#include "BaseballGameMode.h"
#include "BaseballGameState.h"
#include "Baseball/Player/BaseballPlayerController.h"
#include "Baseball/Player/BaseballPlayerState.h"

ABaseballGameMode::ABaseballGameMode()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABaseballGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
			SyncTimerHandle,
			this, 
			&ThisClass::SynchronizeTimer,
			0.25f,
			true
	);
}

void ABaseballGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(NewPlayer);
	if (IsValid(PC))
	{
		PlayerList.Add(PC); 
	}
	
	if (PlayerList.Num() >= 2)
	{
		ResetGame();
	}
}

void ABaseballGameMode::SendMessageToClient(ABaseballPlayerController* ReceivePlayer, const FString& SenderID,
	const FString& Message, FLinearColor Color)
{
	if (IsValid(ReceivePlayer))
	{
		ReceivePlayer->Client_ReceiveMessage(SenderID, Message, Color);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Server] Server Try to send invalid player"));
	}
}

void ABaseballGameMode::BroadcastMessage(const FString& SenderID, const FString& Message, FLinearColor Color)
{
	for (auto PC : PlayerList)
	{
		if (IsValid(PC))
		{
			UE_LOG(LogTemp, Warning, TEXT("[Server] Send Message To Client %s"), *Message);
			SendMessageToClient(PC, SenderID, Message, Color);
		}
	}
}

void ABaseballGameMode::GenerateSecretNumber()
{
	TArray<int32> Pool;
	for (int32 i = 1; i <= 9; ++i) Pool.Add(i);
	
	SecretNumber = TEXT(""); 
	for (int32 i = 0; i < 3; ++i)
	{
		int32 RandomIdx = FMath::RandRange(0, Pool.Num() - 1);
		SecretNumber += FString::FromInt(Pool[RandomIdx]);
		Pool.RemoveAt(RandomIdx);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[Server] Generated Secret Number: %s"), *SecretNumber);
}

bool ABaseballGameMode::IsValidGuessNumber(const FString& GuessNumber)
{
	if (GuessNumber.Len() != 3) return false; 
	
	bool bIsUnique = true;
	TSet<TCHAR> UniqueDigits; 
	for (TCHAR C : GuessNumber)
	{
		if (!FChar::IsDigit(C) || C == '0') return false;  
		if (UniqueDigits.Contains(C)) return false;
		UniqueDigits.Add(C);
	}
	
	return true;
}

bool ABaseballGameMode::JudgeGuessNumber(const FString& GuessNumber)
{
	int32 StrikeCount = 0, BallCount = 0, OutCount = 0;
	for (int32 i = 0; i < 3; ++i)
	{
		if (GuessNumber[i] == SecretNumber[i]) 
		{
			++StrikeCount;
		}
		else if (SecretNumber.Contains(GuessNumber.Mid(i, 1))) 
		{
			++BallCount;
		}
		else
		{
			++OutCount;
		}
	}
	
	const FString StrikeCountString = FString::Printf(TEXT("Strike : %d"), StrikeCount);
	const FString BallCountString = FString::Printf(TEXT("Ball : %d"), BallCount);
	const FString OutCountString = FString::Printf(TEXT("Out : %d"), OutCount);
	
	BroadcastMessage(TEXT("System"), StrikeCountString, FLinearColor::White);
	BroadcastMessage(TEXT("System"), BallCountString, FLinearColor::White);
	BroadcastMessage(TEXT("System"), OutCountString, FLinearColor::White);
	
	return StrikeCount == 3; 
}

void ABaseballGameMode::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(
		TurnTimerHandle,
		this, 
		&ThisClass::OnTimeOut,
		30.f,
		false
	);
}

void ABaseballGameMode::OnTimeOut()
{
	BroadcastMessage(TEXT("System"), TEXT("시간 초과로 턴이 넘어갑니다."), FLinearColor::Yellow);
	SwitchTurn();
}

void ABaseballGameMode::SynchronizeTimer()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(SyncTimerHandle))
	{
		float Remainder = GetWorld()->GetTimerManager().GetTimerRemaining(TurnTimerHandle);
		int32 NewTimeLimit = FMath::CeilToInt(Remainder);
		
		if (ABaseballGameState* GS = GetGameState<ABaseballGameState>())
		{
			if (GS->LimitTime != NewTimeLimit)
			{
				GS->LimitTime = NewTimeLimit;
				UE_LOG(LogTemp, Log, TEXT("[Server] Time Updated. %i"), NewTimeLimit);
			}
		}
	}
}

void ABaseballGameMode::SwitchTurn()
{
	CurrentTurnIndex = (CurrentTurnIndex + 1) % PlayerList.Num();
	ABaseballPlayerController* CurrentTurnPlayer = PlayerList[CurrentTurnIndex];
	if (!IsValid(CurrentTurnPlayer)) return; 
	
	SendMessageToClient(CurrentTurnPlayer,TEXT("System") ,TEXT("당신의 턴입니다. 숫자를 입력해주세요."), FLinearColor::White);
	
	CurrentTurnPlayerState = CurrentTurnPlayer->GetPlayerState<ABaseballPlayerState>();
	CurrentTurnPlayerState->LeftChance = CurrentTurnPlayerState->MaxLeftChance; 
	
	ABaseballGameState* GS = GetGameState<ABaseballGameState>(); 
	if (IsValid(GS))
	{
		GS->CurrentTurnPlayer = CurrentTurnPlayerState; 
	}
	
	StartTimer(); 
}

void ABaseballGameMode::ReceiveGuess(ABaseballPlayerController* Sender, const FString& GuessNumber)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server] Receive PlayerInput %s"), *GuessNumber);
	
	if (!PlayerList.IsValidIndex(CurrentTurnIndex) || PlayerList[CurrentTurnIndex] != Sender)
	{
		SendMessageToClient(
			Sender, 
			TEXT("System"),
			TEXT("당신의 턴이 아닙니다."),
			FLinearColor::Yellow
		); 
		
		return; 
	}
	
	if (!IsValidGuessNumber(GuessNumber))
	{
		SendMessageToClient(
			Sender,
			TEXT("System"), TEXT("잘못된 입력입니다. 다시 입력하세요."), 
			FLinearColor::Yellow
		);
		
		return; 
	}
	
	CurrentTurnPlayerState->LeftChance--; 
	
	if (JudgeGuessNumber(GuessNumber))
	{
		BroadcastMessage(Sender->PlayerState->GetPlayerName(), GuessNumber, FLinearColor::Gray);
		SendMessageToClient(
			Sender,
			TEXT("System"), 
			TEXT("승리!!!"), 
			FLinearColor::Yellow
		);
		
		for (auto Player : PlayerList)
		{
			if (Player != Sender)
			{
				SendMessageToClient(
				Player,
				TEXT("System"), TEXT("패배!!!"), 
				FLinearColor::Yellow
				);
				
			}
		}
		
		ResetGame(); 
	}
	else if (CurrentTurnPlayerState->LeftChance <= 0)
	{
		SwitchTurn();
		return; 
	}
	
	FString LeftChanceString = FString::Printf(TEXT("남은 기회 : %i"), CurrentTurnPlayerState->LeftChance);
	SendMessageToClient(Sender,TEXT("System"), LeftChanceString, FLinearColor::White);
}

void ABaseballGameMode::ResetGame()
{
	GenerateSecretNumber(); 
	
	BroadcastMessage(TEXT("System"),TEXT("게임을 시작합니다."),FLinearColor::White); 
	
	CurrentTurnIndex = -1;
	SwitchTurn(); 
}
