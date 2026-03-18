#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWidget.generated.h"

class UScrollBox;
class UEditableTextBox; 

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageCommitted, const FString&, Message);

UCLASS()
class BASEBALL_API UChatWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnChatCommitted(const FText& Text, ETextCommit::Type CommitMethod);
	
public:
	void AddChatMessage(const FString &Message, FLinearColor Color = FLinearColor::Gray);

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMessageCommitted OnMessageCommitted;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> ChatScrollBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEditableTextBox> EditableTextBox;	
	
};
