// Fill out your copyright notice in the Description page of Project Settings.
#include "UISystem.h"
#include "WidgetInfo.h"
#include "CustomWidget.h"

DEFINE_LOG_CATEGORY(UISystem_Log);

UUISystem::UUISystem()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTable(
		TEXT("/Game/WidgetInfo.WidgetInfo")
	);
	
	if (DataTable.Succeeded() == true)
	{
		m_WidgetTable = DataTable.Object;
	}

	m_ActiveWidgets.Empty();
}

UUISystem::~UUISystem()
{
	m_WidgetTable = nullptr;
	m_ActiveWidgets.Empty();
}

// public ==================================================================

TWeakObjectPtr<UCustomWidget> UUISystem::GetActiveWidget(const FName& InName)
{
	for (UCustomWidget* iter : m_ActiveWidgets)
	{
		if (iter == nullptr || iter->IsValidLowLevel() == false)
		{
			continue;
		}

		if (iter->GetWidgetName() == InName)
		{
			return iter;
		}
	}

	return nullptr;
}

void UUISystem::ShowWidget(const FName& InName)
{
	TWeakObjectPtr<UCustomWidget> Widget = GetActiveWidget(InName);
	if (Widget.IsValid() == true)
	{
		CHHK_LOG(Warning, TEXT("Failed to add widget to viewport. %s is already added to viewport."), *InName.ToString());
		return;
	}
	
	Widget = LoadWidget(InName);
	if (Widget.IsValid() == true)
	{
		Widget.Get()->AddToViewport();
		m_ActiveWidgets.Emplace(Widget.Get());
	}
}

void UUISystem::RemoveWidget(const FName& InName)
{
	for (int32 i = 0; i < m_ActiveWidgets.Num(); ++i)
	{
		UCustomWidget* Widget = m_ActiveWidgets[i];
		if (Widget == nullptr || Widget->IsValidLowLevel() == false)
		{
			continue;
		}

		if (Widget->GetWidgetName() == InName)
		{
			m_ActiveWidgets.RemoveAt(i);
			Widget->RemoveFromParent();
			break;
		}
	}
}

// private =================================================================

UCustomWidget* UUISystem::LoadWidget(const FName& InName)
{
	// check WidgetInfo data table
	if (m_WidgetTable == nullptr || m_WidgetTable->IsValidLowLevel() == false)
	{
		CHHK_LOG(Error, TEXT("WidgetTable is null"));
		return nullptr;
	}

	// get widget info by data table
	const FWidgetInfo* WidgetInfoTable = m_WidgetTable->FindRow<FWidgetInfo>(const_cast<FName&>(InName), InName.ToString(), false);
	if (WidgetInfoTable == nullptr)
	{
		CHHK_LOG(Error, TEXT("Failed to load widget from WidgetTable. Widget name is %s"), *InName.ToString());
		return nullptr;
	}

	// check widget path
	if (WidgetInfoTable->widget_path.IsNull() == true)
	{
		CHHK_LOG(Error, TEXT("Failed to get widget path. Widget path is %s"), *WidgetInfoTable->widget_path.ToString());
		return nullptr;
	}
	
	// load widget
	TSubclassOf<UCustomWidget> WidgetClass = Cast<UClass>(WidgetInfoTable->widget_path.TryLoad());
	if (WidgetClass == nullptr)
	{
		CHHK_LOG(Error, TEXT("Failed to load widget by path. Widget path is %s"), *WidgetInfoTable->widget_path.ToString());
		return nullptr;
	}
	
	// create widget
	UCustomWidget* Widget = CreateWidget<UCustomWidget>(GetWorld(), WidgetClass);
	if (Widget == nullptr || Widget->IsValidLowLevel() == false)
	{
		CHHK_LOG(Error, TEXT("Failed to create widget. Widget name is %s."), *InName.ToString());
		return nullptr;
	}

	Widget->SetWidgetName(WidgetInfoTable->name);
	return Widget;
}