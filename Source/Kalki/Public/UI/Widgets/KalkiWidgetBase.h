// Copyright of V.S. Puranam and no one else

// Public/UI/KalkiWidgetThemedBase.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Logging/KalkiLogTypes.h"
#include "KalkiWidgetBase.generated.h"

class UKalkiWidgetTheme;

/**
 * Base class for ALL Kalki widgets
 * Provides theming, styling, and common functionality
 * All Kalki widgets should inherit from this (directly or indirectly)
 */
UCLASS(Abstract)
class KALKI_API UKalkiWidgetBase : public UUserWidget
{
    GENERATED_BODY()

public:
    // Theme management
    
    /** Set the theme for this widget */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Theme")
    void SetTheme(UKalkiWidgetTheme* InTheme);

    /** Get the current theme */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Theme")
    UKalkiWidgetTheme* GetTheme() const { return WidgetTheme; }

    /** Apply the theme to this widget */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Theme")
    virtual void ApplyTheme();

    // Common styling functions
    
    /** Set primary color override */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Style")
    void SetPrimaryColor(FLinearColor Color);

    /** Set secondary color override */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Style")
    void SetSecondaryColor(FLinearColor Color);

    /** Get themed color for severity level */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Style")
    FLinearColor GetSeverityColor(EKalkiLogSeverity Severity) const;

    // Animation helpers
    
    /** Play show animation (fade in) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Animation")
    virtual void PlayShowAnimation();

    /** Play hide animation (fade out) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Animation")
    virtual void PlayHideAnimation();

    UFUNCTION(BlueprintCallable, Category = "Kalki|Widget|Animation")
    UWidgetAnimation* GetAnimationByName(FName AnimName) const;

protected:
    
    virtual void NativeConstruct() override;
    virtual void NativePreConstruct() override;

    /** Called when theme is applied - override to apply theme to specific widget elements */
    virtual void OnThemeApplied() {}

    /** Theme asset - can be set per widget or use global default */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Widget|Theme")
    TObjectPtr<UKalkiWidgetTheme> WidgetTheme;

    /** Use global theme if no theme is set */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Widget|Theme")
    bool bUseGlobalTheme = true;

    // Cached colors (can be overridden)
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Widget|Theme")
    FLinearColor PrimaryColor;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Widget|Theme")
    FLinearColor SecondaryColor;

    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Widget|Theme")
    FLinearColor AccentColor;
};
