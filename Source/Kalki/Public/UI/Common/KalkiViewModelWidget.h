// Public/UI/KalkiWidgetBase.h
// REPLACE the existing file with this updated version

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiWidgetBase.h"  // Changed from Blueprint/UserWidget.h
#include "UI/Common/KalkiViewModelBase.h"
#include "KalkiViewModelWidget.generated.h"

// class UKalkiViewModelBase;

/**
 * Base class for widgets that use ViewModels
 * Inherits theming from KalkiWidgetThemedBase
 * Use for complex widgets that need to bind to game data
 */
UCLASS(Abstract)
class KALKI_API UKalkiViewModelWidget : public UKalkiWidgetBase  // Changed base class
{
	GENERATED_BODY()

public:
	// ViewModel management
    
	/** Set the ViewModel for this widget */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Widget")
	virtual void SetViewModel(UKalkiViewModelBase* InViewModel);

	/** Get the ViewModel (untyped) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Widget")
	UKalkiViewModelBase* GetViewModelBase() const { return ViewModel; }

	/** Get the ViewModel with type casting (for C++ subclasses) */
	template<typename T>
	T* GetViewModel() const 
	{ 
		return Cast<T>(ViewModel); 
	}

	/** Check if widget has a ViewModel set */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Widget")
	bool HasViewModel() const { return ViewModel != nullptr; }

protected:
	// UUserWidget interface (through KalkiWidgetThemedBase)
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/**
	 * Called when ViewModel is set - override to bind to ViewModel events
	 * This is where you should subscribe to ViewModel delegates
	 */
	virtual void OnViewModelSet() {}

	/**
	 * Called when ViewModel is cleared - override to unbind from ViewModel events
	 * This is where you should unsubscribe from ViewModel delegates
	 */
	virtual void OnViewModelCleared() {}

	/** Reference to the ViewModel */
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Widget")
	TObjectPtr<UKalkiViewModelBase> ViewModel;
};