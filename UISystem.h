// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UISystem.generated.h"

/**
 * 
 */

DECLARE_LOG_CATEGORY_EXTERN(UISystem_Log, Log, All);
#define LOG_CALLINFO (FString(__FUNCTION__) + FString::Printf(TEXT("(%d)"), __LINE__))
#define CHHK_LOG(Verbosity, Format, ...) UE_LOG(UISystem_Log, Verbosity, TEXT("%s - %s"), *LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

class UCustomWidget;
class UDataTable;

UCLASS()
class GAME_N_API UUISystem : public UObject
{
	GENERATED_BODY()

public:
	UUISystem();
	~UUISystem();

	TWeakObjectPtr<UCustomWidget> GetActiveWidget(const FName& InName);	// 화면에 띄워진 위젯 중 가져오기
	void ShowWidget(const FName& InName);								// 화면에 위젯을 그림
	void RemoveWidget(const FName& InName);								// 화면에 그려진 위젯 지우기
	
private:
	UCustomWidget* LoadWidget(const FName& InName);

private:
	UPROPERTY(Transient)
		TArray<UCustomWidget*> m_ActiveWidgets;
	
	UDataTable* m_WidgetTable = nullptr;
};