// Copyright of V.S. Puranam and no one else

// Public/UI/Combat/KalkiCombatUI.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelWidget.h"
#include "KalkiCombatUI.generated.h"

class UKalkiCombatLogWidget;
class UOverlay;

/**
 * Combat Mode UI Container
 * Contains all UI elements for tactical combat
 * Self-contained widget for combat gameplay
 */
UCLASS()
class KALKI_API UKalkiCombatUI : public UKalkiViewModelWidget
{
	GENERATED_BODY()

public:
	/** Called when combat mode becomes active */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Combat UI")
	virtual void OnModeEntered();

	/** Called when combat mode becomes inactive */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Combat UI")
	virtual void OnModeExited();

	/** Get combat log widget */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Combat UI")
	UKalkiCombatLogWidget* GetCombatLog() const { return CombatLogWidget; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Combat widgets (bound in Blueprint or created in C++)

	/** Combat log - displays combat messages */
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UKalkiCombatLogWidget> CombatLogWidget;

	// Future combat widgets (add as needed):
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiTurnOrderWidget> TurnOrderWidget;
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiAbilityBarWidget> AbilityBarWidget;
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiCharacterStatusWidget> CharacterStatusWidget;
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiGridInfoWidget> GridInfoWidget;
};
