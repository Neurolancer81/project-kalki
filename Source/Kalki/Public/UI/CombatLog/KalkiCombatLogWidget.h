// Copyright of V.S. Puranam and no one else

// Public/UI/CombatLog/KalkiCombatLogWidget.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelWidget.h"
#include "KalkiCombatLogWidget.generated.h"

class UKalkiCombatLogViewModel;
class UScrollBox;
class UButton;
class UKalkiLogMessageWidget;
struct FKalkiLogDisplayEntry;

/**
 * Main combat log widget
 * Displays scrollable list of combat messages
 */
UCLASS()
class KALKI_API UKalkiCombatLogWidget : public UKalkiViewModelWidget
{
	GENERATED_BODY()

protected:
	// UKalkiWidgetBase interface
	virtual void OnViewModelSet() override;
	virtual void OnViewModelCleared() override;

	// UI Components
	UPROPERTY(meta = (BindWidget))
	UScrollBox* MessageScrollBox;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenLogButton;

	UPROPERTY(meta = (BindWidget))
	UButton* OpenFolderButton;

	// Message widget class (set in Blueprint)
	UPROPERTY(EditDefaultsOnly, Category = "Kalki|CombatLog")
	TSubclassOf<UKalkiLogMessageWidget> MessageWidgetClass;

	// Event handlers
	UFUNCTION()
	void OnCombatLogUpdated();

	UFUNCTION()
	void OnOpenLogClicked();

	UFUNCTION()
	void OnOpenFolderClicked();

	// Refresh the message display
	void RefreshMessages();

	// Add a single message widget
	void AddMessageWidget(const FKalkiLogDisplayEntry& Entry);
};