// Copyright of V.S. Puranam and no one else

// Private/UI/Strategy/KalkiStrategyUI.cpp

#include "UI/Strategy/KalkiStrategyUI.h"
#include "Logging/KalkiLog.h"

void UKalkiStrategyUI::NativeConstruct()
{
	Super::NativeConstruct();

	KalkiLog::System(TEXT("StrategyUI constructed"), EKalkiLogSeverity::Log, this);
}

void UKalkiStrategyUI::NativeDestruct()
{
	Super::NativeDestruct();

	KalkiLog::System(TEXT("StrategyUI destructed"), EKalkiLogSeverity::Log, this);
}

void UKalkiStrategyUI::OnModeEntered()
{
	KalkiLog::System(TEXT("Strategy mode entered"), EKalkiLogSeverity::Log, this);

	// TODO: Initialize strategy widgets when implemented
	// - Refresh campaign map
	// - Update mission list
	// - Refresh roster
	// - Update resources display
}

void UKalkiStrategyUI::OnModeExited()
{
	KalkiLog::System(TEXT("Strategy mode exited"), EKalkiLogSeverity::Log, this);

	// TODO: Cleanup strategy widgets (if needed)
}