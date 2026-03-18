#include "BaseballHUD.h"
#include "BaseballMainWidget.h"
#include "ChatWidget.h"
#include "Components/EditableTextBox.h"


void ABaseballHUD::BeginPlay()
{
	Super::BeginPlay();
	
	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UBaseballMainWidget>(GetOwningPlayerController(),MainWidgetClass);
		if (MainWidget)
		{
			MainWidget->AddToViewport(); 
		}
		
		if (MainWidget->ChatWidget && MainWidget->ChatWidget->EditableTextBox)
		{
			MainWidget->ChatWidget->EditableTextBox->SetFocus();
		}
	}
}

void ABaseballHUD::UpdateChat(const FString& Message, FLinearColor Color)
{
	if (MainWidget)
	{
		MainWidget->ChatWidget->AddChatMessage(Message, Color);
	}
}

void ABaseballHUD::UpdateRemainTime(int32 RemainTime)
{
	if (MainWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("[HUD] Timer Update: %i"), RemainTime);
		MainWidget->UpdateLimitTime(RemainTime);
	}
}


