// Copyright of V.S. Puranam and no one else

// Private/UI/Common/KalkiHUD.cpp

#include "UI/Common/KalkiHUD.h"
#include "UI/Common/KalkiUIManager.h"
#include "UI/Common/KalkiWidgetTheme.h"
#include "UI/Combat/KalkiCombatUI.h"
#include "UI/Strategy/KalkiStrategyUI.h"
#include "UI/CombatLog/KalkiCombatLogViewModel.h"
#include "Logging/KalkiLog.h"

AKalkiHUD::AKalkiHUD()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AKalkiHUD::BeginPlay()
{
    Super::BeginPlay();

    // Get UIManager
    UIManager = GetWorld()->GetSubsystem<UKalkiUIManager>();
    if (!UIManager)
    {
        KalkiLog::System(TEXT("KalkiHUD - Failed to get UIManager"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Register ViewModels
    UIManager->RegisterViewModelClass(UKalkiCombatLogViewModel::StaticClass());

    // Initialize ViewModels
    UIManager->InitializeViewModels();

    // Set global theme
    if (DefaultTheme)
    {
        UIManager->SetGlobalTheme(DefaultTheme);
    }

    // Create mode UIs
    CreateModeUIs();

    KalkiLog::System(TEXT("KalkiHUD initialized"), EKalkiLogSeverity::Log, this);
}

void AKalkiHUD::CreateModeUIs()
{
    if (!UIManager)
    {
        return;
    }

    // Create Strategy UI
    if (StrategyUIClass)
    {
        StrategyUI = CreateWidget<UKalkiStrategyUI>(GetWorld(), StrategyUIClass);
        if (StrategyUI)
        {
            StrategyUI->AddToViewport(0);
            StrategyUI->SetVisibility(ESlateVisibility::Collapsed);
            KalkiLog::System(TEXT("KalkiHUD - Strategy UI created (collapsed)"), EKalkiLogSeverity::Log, this);
        }
        else
        {
            KalkiLog::System(TEXT("KalkiHUD - Failed to create Strategy UI"), EKalkiLogSeverity::Error, this);
        }
    }
    else
    {
        KalkiLog::System(TEXT("KalkiHUD - StrategyUIClass not set"), EKalkiLogSeverity::Warning, this);
    }

    // Create Combat UI (with ViewModel)
    if (CombatUIClass)
    {
        CombatUI = UIManager->CreateWidgetWithViewModel<UKalkiCombatUI, UKalkiCombatLogViewModel>(CombatUIClass);

        if (CombatUI)
        {
            CombatUI->AddToViewport(0);
            CombatUI->SetVisibility(ESlateVisibility::Collapsed);
            KalkiLog::System(TEXT("KalkiHUD - Combat UI created (collapsed)"), EKalkiLogSeverity::Log, this);
        }
        else
        {
            KalkiLog::System(TEXT("KalkiHUD - Failed to create Combat UI"), EKalkiLogSeverity::Error, this);
        }
    }
    else
    {
        KalkiLog::System(TEXT("KalkiHUD - CombatUIClass not set"), EKalkiLogSeverity::Warning, this);
    }
}

void AKalkiHUD::SwitchToCombatMode()
{
    HideAllModeUIs();

    if (CombatUI)
    {
        CombatUI->SetVisibility(ESlateVisibility::Visible);
        CombatUI->OnModeEntered();
        CurrentMode = EKalkiGameMode::Combat;
        KalkiLog::System(TEXT("Switched to Combat Mode"), EKalkiLogSeverity::Log, this);
    }
    else
    {
        KalkiLog::System(TEXT("Cannot switch to Combat Mode - CombatUI is null"), EKalkiLogSeverity::Error, this);
    }
}

void AKalkiHUD::SwitchToStrategyMode()
{
    HideAllModeUIs();

    if (StrategyUI)
    {
        StrategyUI->SetVisibility(ESlateVisibility::Visible);
        StrategyUI->OnModeEntered();
        CurrentMode = EKalkiGameMode::Strategy;
        KalkiLog::System(TEXT("Switched to Strategy Mode"), EKalkiLogSeverity::Log, this);
    }
    else
    {
        KalkiLog::System(TEXT("Cannot switch to Strategy Mode - StrategyUI is null"), EKalkiLogSeverity::Error, this);
    }
}

void AKalkiHUD::HideAllModeUIs()
{
    if (StrategyUI)
    {
        StrategyUI->SetVisibility(ESlateVisibility::Collapsed);
        StrategyUI->OnModeExited();
    }

    if (CombatUI)
    {
        CombatUI->SetVisibility(ESlateVisibility::Collapsed);
        CombatUI->OnModeExited();
    }
}