// Copyright of V.S. Puranam and no one else

// Public/UI/KalkiUIManager.h

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Logging/KalkiLog.h"
#include "Subsystems/WorldSubsystem.h"
#include "KalkiUIManager.generated.h"

class UKalkiWidgetTheme;
class UKalkiViewModelBase;
class UKalkiViewModelWidget;

/**
 * Central manager for all UI in the game
 * Creates and owns all ViewModels
 * Provides interface for creating widgets with ViewModels bound
 */
UCLASS()
class KALKI_API UKalkiUIManager : public UWorldSubsystem
{
    GENERATED_BODY()

public:
    // Subsystem lifecycle
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // ViewModel management
    
    /**
     * Get a ViewModel by type
     * Returns nullptr if ViewModel hasn't been registered
     */
    template<typename T>
    T* GetViewModel() const
    {
        static_assert(TIsDerivedFrom<T, UKalkiViewModelBase>::Value, "T must derive from UKalkiViewModelBase");
        
        if (TObjectPtr<UKalkiViewModelBase> const* FoundVM = ViewModels.Find(T::StaticClass()))
        {
            return Cast<T>(*FoundVM);
        }
        
        return nullptr;
    }

    /**
     * Register a ViewModel type to be created and managed
     * Call this before Initialize() to register ViewModels
     */
    UFUNCTION(BlueprintCallable, Category = "Kalki|UI")
    void RegisterViewModelClass(TSubclassOf<UKalkiViewModelBase> ViewModelClass);

    // Widget creation helpers
    
    /**
     * Create a widget and automatically bind the appropriate ViewModel
     * Template version for C++
     */
    template<typename TWidget, typename TViewModel>
    TWidget* CreateWidgetWithViewModel(TSubclassOf<TWidget> WidgetClass)
    {
        static_assert(TIsDerivedFrom<TWidget, UKalkiViewModelWidget>::Value, "TWidget must derive from UKalkiViewModelWidget");
        static_assert(TIsDerivedFrom<TViewModel, UKalkiViewModelBase>::Value, "TViewModel must derive from UKalkiViewModelBase");

        TWidget* Widget = CreateWidget<TWidget>(GetWorld(), WidgetClass);
        if (Widget)
        {
            if (TViewModel* VM = GetViewModel<TViewModel>())
            {
                Widget->SetViewModel(VM);
            }
            else
            {
                KalkiLog::System(
                    FString::Printf(TEXT("CreateWidgetWithViewModel - ViewModel %s not found"), 
                        *TViewModel::StaticClass()->GetName()),
                    EKalkiLogSeverity::Warning,
                    this
                );
            }
        }
        
        return Widget;
    }

    /**
     * Create a widget and bind ViewModel (Blueprint-callable)
     */
    UFUNCTION(BlueprintCallable, Category = "Kalki|UI", meta = (DeterminesOutputType = "WidgetClass"))
    UKalkiViewModelWidget* CreateWidgetWithViewModelByClass(
        TSubclassOf<UKalkiViewModelWidget> WidgetClass,
        TSubclassOf<UKalkiViewModelBase> ViewModelClass);

    /** Get the global theme for all widgets */
    UFUNCTION(BlueprintCallable, Category = "Kalki|UI|Theme")
    UKalkiWidgetTheme* GetGlobalTheme() const { return GlobalTheme; }

    /** Set the global theme for all widgets */
    UFUNCTION(BlueprintCallable, Category = "Kalki|UI|Theme")
    void SetGlobalTheme(UKalkiWidgetTheme* InTheme);

    /** Initialize all registered ViewModels (call after registration) */
    UFUNCTION(BlueprintCallable, Category = "Kalki|UI")
    void InitializeViewModels();

protected:

    /** Global theme used by all widgets unless overridden */
    UPROPERTY(EditDefaultsOnly, Category = "Kalki|UI|Theme")
    TObjectPtr<UKalkiWidgetTheme> GlobalTheme;

    /** Shutdown all ViewModels */
    void ShutdownViewModels();

    /** Map of ViewModel class to instance */
    UPROPERTY()
    TMap<TSubclassOf<UKalkiViewModelBase>, TObjectPtr<UKalkiViewModelBase>> ViewModels;

    /** List of ViewModel classes to create on initialization */
    UPROPERTY()
    TArray<TSubclassOf<UKalkiViewModelBase>> RegisteredViewModelClasses;
};