// Copyright of V.S. Puranam and no one else

// Public/UI/KalkiSimpleWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/KalkiWidgetBase.h"
#include "KalkiSimpleWidget.generated.h"

/**
 * Base class for simple display widgets that don't need ViewModels
 * Inherits theming from KalkiWidgetThemedBase
 * Use for: message displays, item slots, health bars, status icons, etc.
 */
UCLASS(Abstract)
class KALKI_API UKalkiSimpleWidget : public UKalkiWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/**
	 * Helper function to trigger display update
	 * Call this when data changes
	 */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Widget")
	void UpdateDisplay();

	/**
	 * Override to handle display updates
	 * Called when UpdateDisplay() is called
	 */
	virtual void OnDisplayUpdate() {}
};
