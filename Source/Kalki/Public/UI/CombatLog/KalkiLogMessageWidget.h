// Copyright of V.S. Puranam and no one else

// Public/UI/CombatLog/KalkiLogMessageWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/KalkiSimpleWidget.h"  // Changed from Blueprint/UserWidget.h
#include "UI/CombatLog/KalkiCombatLogTypes.h"
#include "KalkiLogMessageWidget.generated.h"

class UTextBlock;

/**
 * Widget for displaying a single log message
 * Inherits theming support from KalkiSimpleWidget
 */
UCLASS()
class KALKI_API UKalkiLogMessageWidget : public UKalkiSimpleWidget  // Changed base class
{
	GENERATED_BODY()

public:
	/** Set the message to display */
	UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
	void SetMessage(const FKalkiLogDisplayEntry& InEntry);

protected:
	// Override theme application
	virtual void OnThemeApplied() override;

	// UI Components
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MessageText;

	// Stored message entry (for future tooltip implementation)
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|CombatLog")
	FKalkiLogDisplayEntry MessageEntry;

	// Hover events (for future tooltip implementation)
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
};