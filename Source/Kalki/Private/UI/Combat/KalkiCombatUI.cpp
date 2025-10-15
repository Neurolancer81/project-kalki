// Copyright of V.S. Puranam and no one else

// Private/UI/Combat/KalkiCombatUI.cpp

#include "UI/Combat/KalkiCombatUI.h"
#include "UI/CombatLog/KalkiCombatLogWidget.h"
#include "Logging/KalkiLog.h"

void UKalkiCombatUI::NativeConstruct()
{
	Super::NativeConstruct();

	// Pass ViewModel to child widget (CombatLogWidget)
	if (CombatLogWidget)
	{
		if (UKalkiViewModelBase* VM = GetViewModel<UKalkiViewModelBase>())
		{
			CombatLogWidget->SetViewModel(VM);
		}
	}

	KalkiLog::System(TEXT("CombatUI constructed"), EKalkiLogSeverity::Log, this);
}

void UKalkiCombatUI::NativeDestruct()
{
	Super::NativeDestruct();

	KalkiLog::System(TEXT("CombatUI destructed"), EKalkiLogSeverity::Log, this);
}

void UKalkiCombatUI::OnModeEntered()
{
	KalkiLog::System(TEXT("Combat mode entered"), EKalkiLogSeverity::Log, this);

	// Initialize combat widgets
	if (CombatLogWidget)
	{
		KalkiLog::System(TEXT("Combat log ready"), EKalkiLogSeverity::Log, this);
	}

	// Future: Initialize other combat widgets
	// if (TurnOrderWidget) TurnOrderWidget->RefreshTurnOrder();
	// if (AbilityBarWidget) AbilityBarWidget->RefreshAbilities();
}

void UKalkiCombatUI::OnModeExited()
{
	KalkiLog::System(TEXT("Combat mode exited"), EKalkiLogSeverity::Log, this);

	// Cleanup combat widgets (if needed)
	// Most widgets just hide and retain state
}