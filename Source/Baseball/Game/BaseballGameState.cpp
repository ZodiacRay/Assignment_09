#include "BaseballGameState.h"

#include "Baseball/Player/BaseballPlayerController.h"
#include "Baseball/UI/BaseballHUD.h"
#include "Net/UnrealNetwork.h"

ABaseballGameState::ABaseballGameState()
{
	
}

void ABaseballGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ABaseballGameState, LimitTime);
	DOREPLIFETIME(ABaseballGameState, CurrentTurnPlayer);
}

void ABaseballGameState::OnRep_CurrentTurnPlayer()
{
	
}


void ABaseballGameState::OnRep_LimitTime()
{
	if (ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(GetWorld()->GetFirstPlayerController()))
	{
		if(ABaseballHUD* BaseBallHUD = Cast<ABaseballHUD>(PC->GetHUD()))
		{
			BaseBallHUD->UpdateRemainTime(LimitTime);
		}
	}
}
