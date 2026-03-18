#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BaseballMainWidget.generated.h"

class UTextBlock;
class UChatWidget;

UCLASS()
class BASEBALL_API UBaseballMainWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void AddChatMessage(const FString& Message);
	
	UFUNCTION()
	void UpdateLimitTime(int32 LimitTime); 

public:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UChatWidget> ChatWidget;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> LimitTimeText;
};
