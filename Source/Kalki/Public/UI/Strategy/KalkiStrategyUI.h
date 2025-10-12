// Copyright of V.S. Puranam and no one else

// Public/UI/Strategy/KalkiStrategyUI.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/KalkiViewModelWidget.h"
#include "KalkiStrategyUI.generated.h"

/**
 * Strategy Mode UI Container
 * Contains all UI elements for strategy layer (campaign map, roster, etc.)
 * Placeholder for now - will be expanded when strategy layer is implemented
 */
UCLASS()
class KALKI_API UKalkiStrategyUI : public UKalkiViewModelWidget
{
	GENERATED_BODY()

public:
	/** Called when strategy mode becomes active */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Strategy UI")
	virtual void OnModeEntered();

	/** Called when strategy mode becomes inactive */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Strategy UI")
	virtual void OnModeExited();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	// Future strategy widgets (add as needed):
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiCampaignMapWidget> CampaignMapWidget;
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiMissionListWidget> MissionListWidget;
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiRosterWidget> RosterWidget;
    
	// UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	// TObjectPtr<UKalkiResourceWidget> ResourceWidget;
};