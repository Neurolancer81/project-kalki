// Copyright of V.S. Puranam and no one else

// Public/UI/KalkiViewModelBase.h

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KalkiViewModelBase.generated.h"

/**
 * Base class for all ViewModels in the Kalki UI architecture
 * ViewModels act as intermediaries between game data (Model) and UI (View)
 * They transform game data into UI-friendly formats and handle UI logic
 */
UCLASS(Abstract, Blueprintable)
class KALKI_API UKalkiViewModelBase : public UObject
{
    GENERATED_BODY()

public:
    UKalkiViewModelBase();

    // Lifecycle management
    
    /** Initialize the ViewModel with world context */
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    virtual void Initialize(UWorld* InWorld);

    /** Shutdown and cleanup */
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    virtual void Shutdown();

    /** Check if ViewModel is initialized */
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    bool IsInitialized() const { return bInitialized; }

    // Enable/Disable
    
    /** Enable/disable this ViewModel (disabled ViewModels don't update) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    virtual void SetEnabled(bool bInEnabled);

    /** Check if ViewModel is enabled */
    UFUNCTION(BlueprintCallable, Category = "Kalki|ViewModel")
    bool IsEnabled() const { return bEnabled; }

    // World access
    virtual UWorld* GetWorld() const override;

    /** Called when ViewModel is initialized - override to set up bindings */
    virtual void OnInitialize();

    /** Called when ViewModel is shut down - override to clean up bindings */
    virtual void OnShutdown();

protected:

    /** Called when enabled state changes */
    virtual void OnEnabledChanged(bool bNewEnabled) {}

    /** World context for accessing subsystems and game state */
    UPROPERTY()
    TObjectPtr<UWorld> WorldContext;

    /** Whether this ViewModel has been initialized */
    UPROPERTY()
    bool bInitialized = false;

    /** Whether this ViewModel is currently enabled */
    UPROPERTY()
    bool bEnabled = true;
};