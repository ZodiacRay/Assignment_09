#include "ChatWidget.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"

void UChatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (EditableTextBox)
	{
		EditableTextBox->OnTextCommitted.AddDynamic(this, &UChatWidget::OnChatCommitted);
	}
}

void UChatWidget::OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::OnEnter && !Text.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("[ChatWidget] Message Commited")); 
		OnMessageCommitted.Broadcast(Text.ToString());
        
		EditableTextBox->SetText(FText::GetEmpty());
		EditableTextBox->SetFocus();
	}
}

void UChatWidget::AddChatMessage(const FString& Message, FLinearColor Color)
{
	if (!ChatScrollBox) return;
	
	UTextBlock* NewMessage = NewObject<UTextBlock>(ChatScrollBox);
	if (NewMessage)
	{
		NewMessage->SetText(FText::FromString(Message));
		
		FSlateFontInfo FontInfo = NewMessage->GetFont();
		FontInfo.Size = 30;
		NewMessage->SetFont(FontInfo);
		NewMessage->SetColorAndOpacity(Color);
		
		ChatScrollBox->AddChild(NewMessage);
		ChatScrollBox->ScrollToEnd();
	}
}
