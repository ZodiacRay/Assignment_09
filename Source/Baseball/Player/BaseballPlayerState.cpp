#include "BaseballPlayerState.h"
#include "Net/UnrealNetwork.h"

ABaseballPlayerState::ABaseballPlayerState()
{
	bReplicates = true;
}

void ABaseballPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ThisClass,PlayerName);
	DOREPLIFETIME(ThisClass,LeftChance);
	
}
