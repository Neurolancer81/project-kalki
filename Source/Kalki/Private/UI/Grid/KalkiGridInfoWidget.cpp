// Copyright of V.S. Puranam and no one else

// Private/UI/Grid/KalkiGridInfoWidget.cpp

#include "UI/Grid/KalkiGridInfoWidget.h"
#include "UI/Grid/KalkiGridInfoViewModel.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Logging/KalkiLog.h"

UKalkiGridInfoWidget::UKalkiGridInfoWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
}

void UKalkiGridInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Create ViewModel if not already set
    if (!GridInfoViewModel)
    {
        GridInfoViewModel = NewObject<UKalkiGridInfoViewModel>(this);
        KalkiLog::UI(TEXT("GridInfoWidget - ViewModel created"), EKalkiLogSeverity::Log, this);
    }

    // Initial update
    UpdateFromViewModel();
}

void UKalkiGridInfoWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // Update UI every frame (in case ViewModel changed)
    // For optimization, you could add a "dirty flag" to only update when needed
    UpdateFromViewModel();
}

void UKalkiGridInfoWidget::UpdateFromViewModel()
{
    if (!GridInfoViewModel)
    {
        return;
    }

    // Update coordinate text
    if (CoordinateText)
    {
        CoordinateText->SetText(GridInfoViewModel->GetCoordinateText());
    }

    // Update elevation text
    if (ElevationText)
    {
        ElevationText->SetText(GridInfoViewModel->GetElevationText());
    }

    // Update walkable text (with color coding)
    if (WalkableText)
    {
        WalkableText->SetText(GridInfoViewModel->GetWalkableText());
        
        // Color code based on walkability
        FLinearColor TextColor = GridInfoViewModel->GetWalkableColor();
        WalkableText->SetColorAndOpacity(FSlateColor(TextColor));
    }

    // Update movement cost text (optional, for Phase 4)
    if (MovementCostText && GridInfoViewModel->bShowMovementInfo)
    {
        MovementCostText->SetText(GridInfoViewModel->GetMovementCostText());
        MovementCostText->SetVisibility(ESlateVisibility::Visible);
        
        // Color code based on range
        FLinearColor RangeColor = GridInfoViewModel->GetRangeColor();
        MovementCostText->SetColorAndOpacity(FSlateColor(RangeColor));
    }
    else if (MovementCostText)
    {
        MovementCostText->SetVisibility(ESlateVisibility::Collapsed);
    }

    // Update combat info text (optional, for Phase 6)
    if (CombatInfoText && GridInfoViewModel->bShowCombatInfo)
    {
        // Build combat info string
        FString CombatInfo = FString::Printf(
            TEXT("Cover: %.0f%% | LOS: %s"),
            GridInfoViewModel->CoverBonus * 100.0f,
            GridInfoViewModel->bHasLineOfSight ? TEXT("Yes") : TEXT("No")
        );
        
        CombatInfoText->SetText(FText::FromString(CombatInfo));
        CombatInfoText->SetVisibility(ESlateVisibility::Visible);
    }
    else if (CombatInfoText)
    {
        CombatInfoText->SetVisibility(ESlateVisibility::Collapsed);
    }
}

void UKalkiGridInfoWidget::SetViewModel(UKalkiViewModelBase* InViewModel)
{
    // Call parent implementation (important!)
    Super::SetViewModel(InViewModel);

    // Cast to specific type
    GridInfoViewModel = Cast<UKalkiGridInfoViewModel>(InViewModel);

    if (!GridInfoViewModel)
    {
        if (InViewModel)
        {
            // Wrong type passed
            KalkiLog::UI(
                FString::Printf(TEXT("GridInfoWidget - ViewModel is wrong type: %s (expected UKalkiGridInfoViewModel)"),
                    *InViewModel->GetClass()->GetName()),
                EKalkiLogSeverity::Error,
                this
            );
        }
        else
        {
            // Null passed (might be intentional to clear)
            KalkiLog::UI(TEXT("GridInfoWidget - ViewModel cleared (null)"), EKalkiLogSeverity::Log, this);
        }
        return;
    }

    // Immediate update with new ViewModel
    UpdateFromViewModel();

    KalkiLog::UI(TEXT("GridInfoWidget - GridInfoViewModel set successfully"), EKalkiLogSeverity::Log, this);
}