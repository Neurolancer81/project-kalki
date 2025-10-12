// Copyright of V.S. Puranam and no one else

// Public/UI/Common/KalkiHUD.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "KalkiHUD.generated.h"

class UKalkiUIManager;
class UKalkiCombatUI;
class UKalkiStrategyUI;
class UKalkiWidgetTheme;
class UKalkiCombatLogViewModel;

// Game mode enum
UENUM(BlueprintType)
enum class EKalkiGameMode : uint8
{
    None        UMETA(DisplayName = "None"),
    Strategy    UMETA(DisplayName = "Strategy"),
    Combat      UMETA(DisplayName = "Combat")
};

/**
 * Main HUD for Kalki/Four Ages
 * Manages mode-based UI (Strategy vs Combat)
 * Orchestrates mode switching and widget lifecycle
 */
UCLASS()
class KALKI_API AKalkiHUD : public AHUD
{
    GENERATED_BODY()

public:
    AKalkiHUD();

    // Mode switching
    
    /** Switch to strategy mode (campaign map, roster management, etc.) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|HUD")
    void SwitchToStrategyMode();

    /** Switch to combat mode (tactical grid combat) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|HUD")
    void SwitchToCombatMode();

    /** Get current game mode */
    UFUNCTION(BlueprintCallable, Category = "Kalki|HUD")
    EKalkiGameMode GetCurrentMode() const { return CurrentMode; }

    // Mode UI access
    
    /** Get strategy UI (may be null if not created yet) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|HUD")
    UKalkiStrategyUI* GetStrategyUI() const { return StrategyUI; }

    /** Get combat UI (may be null if not created yet) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|HUD")
    UKalkiCombatUI* GetCombatUI() const { return CombatUI; }

protected:
    virtual void BeginPlay() override;

    /** Create all mode UIs */
    void CreateModeUIs();

    /** Hide all mode UIs */
    void HideAllModeUIs();

    // Current state
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|HUD")
    EKalkiGameMode CurrentMode = EKalkiGameMode::None;

    // Mode UI references
    UPROPERTY()
    TObjectPtr<UKalkiStrategyUI> StrategyUI;

    UPROPERTY()
    TObjectPtr<UKalkiCombatUI> CombatUI;

    // Widget classes (set in BP_KalkiHUD)
    UPROPERTY(EditDefaultsOnly, Category = "Kalki|HUD|Mode UIs")
    TSubclassOf<UKalkiStrategyUI> StrategyUIClass;

    UPROPERTY(EditDefaultsOnly, Category = "Kalki|HUD|Mode UIs")
    TSubclassOf<UKalkiCombatUI> CombatUIClass;

    // Theme
    UPROPERTY(EditDefaultsOnly, Category = "Kalki|HUD|Theme")
    TObjectPtr<UKalkiWidgetTheme> DefaultTheme;

    // UIManager reference
    UPROPERTY()
    TObjectPtr<UKalkiUIManager> UIManager;
};