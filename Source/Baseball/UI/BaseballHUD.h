#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseballHUD.generated.h"

class UBaseballMainWidget;

UCLASS()
class BASEBALL_API ABaseballHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	
public:
	void UpdateChat(const FString& Message, FLinearColor Color);
	
	void UpdateRemainTime(int32 RemainTime);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UBaseballMainWidget> MainWidget;
};
