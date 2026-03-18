#include "BaseballMainWidget.h"
#include "ChatWidget.h"
#include "Baseball/Player/BaseballPlayerController.h"
#include "Components/TextBlock.h"

void UBaseballMainWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (ChatWidget)
	{
		ChatWidget->OnMessageCommitted.AddDynamic(this, &ThisClass::AddChatMessage); 
	}
}

void UBaseballMainWidget::AddChatMessage(const FString& Message)
{
	if (ABaseballPlayerController* PC = Cast<ABaseballPlayerController>(GetOwningPlayer()))
	{
		PC->Server_SendMessage(Message);
	}
}

void UBaseballMainWidget::UpdateLimitTime(int32 LimitTime)
{
	if (LimitTimeText)
	{
		const FString LimitTimeStr = FString::Printf(TEXT("%d s"), LimitTime);
		
		LimitTimeText->SetText(FText::FromString(LimitTimeStr));
	}
	
}
