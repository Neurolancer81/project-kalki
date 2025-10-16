// Copyright of V.S. Puranam and no one else

// Public/UI/Grid/KalkiGridInfoWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelWidget.h"
#include "KalkiGridInfoWidget.generated.h"

class UKalkiGridInfoViewModel;
class UTextBlock;
class UBorder;

/**
 * Grid Info Widget
 * Displays information about a grid tile
 * Used for hover tooltips and selection panels
 * 
 * USAGE:
 * - GridVisualizer creates and manages this widget
 * - ViewModel is updated when tile is hovered/selected
 * - Widget automatically updates via MVVM binding
 */
UCLASS()
class KALKI_API UKalkiGridInfoWidget : public UKalkiViewModelWidget
{
    GENERATED_BODY()

public:
    // Constructor
    UKalkiGridInfoWidget(const FObjectInitializer& ObjectInitializer);

protected:
    // ========================================
    // UI ELEMENTS (Bind in UMG Designer)
    // ========================================

    // Main container
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UBorder> RootBorder;

    // Basic tile info
    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> CoordinateText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> ElevationText;

    UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
    TObjectPtr<UTextBlock> WalkableText;

    // Optional: Movement info (Phase 4)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> MovementCostText;

    // Optional: Combat info (Phase 6)
    UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
    TObjectPtr<UTextBlock> CombatInfoText;

    // ========================================
    // VIEWMODEL
    // ========================================

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|ViewModel")
    TObjectPtr<UKalkiGridInfoViewModel> GridInfoViewModel;

    // ========================================
    // WIDGET LIFECYCLE
    // ========================================

    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    // ========================================
    // VIEWMODEL BINDING
    // ========================================

    // Update UI from ViewModel (called every frame if ViewModel changed)
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Info")
    void UpdateFromViewModel();

public:
    // ========================================
    // PUBLIC API
    // ========================================

    // Set the ViewModel for this widget

    virtual void SetViewModel(UKalkiViewModelBase* InViewModel) override;

    // Get the current ViewModel
    UFUNCTION(BlueprintPure, Category = "Kalki|Grid Info")
    UKalkiGridInfoViewModel* GetViewModel() const { return GridInfoViewModel; }
};