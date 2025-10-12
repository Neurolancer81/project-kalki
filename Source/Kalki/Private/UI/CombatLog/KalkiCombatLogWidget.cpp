// Copyright of V.S. Puranam and no one else

// Private/UI/CombatLog/KalkiCombatLogWidget.cpp

#include "UI/CombatLog/KalkiCombatLogWidget.h"
#include "UI/CombatLog/KalkiCombatLogViewModel.h"
#include "UI/CombatLog/KalkiLogMessageWidget.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Logging/KalkiLog.h"

void UKalkiCombatLogWidget::OnViewModelSet()
{
    Super::OnViewModelSet();

    UKalkiCombatLogViewModel* CombatLogVM = GetViewModel<UKalkiCombatLogViewModel>();
    if (!CombatLogVM)
    {
        KalkiLog::System(TEXT("CombatLogWidget - Invalid ViewModel type"), EKalkiLogSeverity::Error, this);
        return;
    }

    KalkiLog::System(
    FString::Printf(TEXT("CombatLogWidget - ViewModel bound successfully. Current messages: %d"), 
        CombatLogVM->GetMessageCount()),
    EKalkiLogSeverity::Log, 
    this);

    // Bind to ViewModel events
    CombatLogVM->OnCombatLogUpdated.AddDynamic(this, &UKalkiCombatLogWidget::OnCombatLogUpdated);

    // Bind button events
    if (OpenLogButton)
    {
        OpenLogButton->OnClicked.AddDynamic(this, &UKalkiCombatLogWidget::OnOpenLogClicked);
    }

    if (OpenFolderButton)
    {
        OpenFolderButton->OnClicked.AddDynamic(this, &UKalkiCombatLogWidget::OnOpenFolderClicked);
    }

    // Initial display
    RefreshMessages();

    KalkiLog::System(TEXT("CombatLogWidget - ViewModel bound"), EKalkiLogSeverity::Log, this);
}

void UKalkiCombatLogWidget::OnViewModelCleared()
{
    UKalkiCombatLogViewModel* CombatLogVM = GetViewModel<UKalkiCombatLogViewModel>();
    if (CombatLogVM)
    {
        CombatLogVM->OnCombatLogUpdated.RemoveDynamic(this, &UKalkiCombatLogWidget::OnCombatLogUpdated);
    }

    if (OpenLogButton)
    {
        OpenLogButton->OnClicked.RemoveDynamic(this, &UKalkiCombatLogWidget::OnOpenLogClicked);
    }

    if (OpenFolderButton)
    {
        OpenFolderButton->OnClicked.RemoveDynamic(this, &UKalkiCombatLogWidget::OnOpenFolderClicked);
    }

    Super::OnViewModelCleared();
}

void UKalkiCombatLogWidget::OnCombatLogUpdated()
{
    RefreshMessages();
}

void UKalkiCombatLogWidget::RefreshMessages()
{
    if (!MessageScrollBox)
    {
        KalkiLog::System(TEXT("CombatLogWidget - MessageScrollBox not bound"), EKalkiLogSeverity::Error, this);
        return;
    }

    UKalkiCombatLogViewModel* CombatLogVM = GetViewModel<UKalkiCombatLogViewModel>();
    if (!CombatLogVM)
    {
        return;
    }

    // Clear existing messages
    MessageScrollBox->ClearChildren();

    // Add all messages from ViewModel
    const TArray<FKalkiLogDisplayEntry>& Messages = CombatLogVM->GetCombatMessages();
    for (const FKalkiLogDisplayEntry& Entry : Messages)
    {
        AddMessageWidget(Entry);
    }

    // Auto-scroll to bottom (newest message)
    MessageScrollBox->ScrollToEnd();
}

void UKalkiCombatLogWidget::AddMessageWidget(const FKalkiLogDisplayEntry& Entry)
{
    if (!MessageWidgetClass)
    {
        KalkiLog::System(TEXT("CombatLogWidget - MessageWidgetClass not set"), EKalkiLogSeverity::Error, this);
        return;
    }

    UKalkiLogMessageWidget* MessageWidget = CreateWidget<UKalkiLogMessageWidget>(this, MessageWidgetClass);
    if (MessageWidget)
    {
        MessageWidget->SetMessage(Entry);
        MessageScrollBox->AddChild(MessageWidget);
    }
}

void UKalkiCombatLogWidget::OnOpenLogClicked()
{
    UKalkiCombatLogViewModel* CombatLogVM = GetViewModel<UKalkiCombatLogViewModel>();
    if (CombatLogVM)
    {
        CombatLogVM->OpenCurrentLogFile();
    }
}

void UKalkiCombatLogWidget::OnOpenFolderClicked()
{
    UKalkiCombatLogViewModel* CombatLogVM = GetViewModel<UKalkiCombatLogViewModel>();
    if (CombatLogVM)
    {
        CombatLogVM->OpenLogFolder();
    }
}