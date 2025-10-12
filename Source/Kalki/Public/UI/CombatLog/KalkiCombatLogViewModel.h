// Copyright of V.S. Puranam and no one else

// Public/UI/CombatLog/KalkiCombatLogViewModel.h

#pragma once

#include "CoreMinimal.h"
#include "UI/Common/KalkiViewModelBase.h"
#include "UI/CombatLog/KalkiCombatLogTypes.h"
#include "Logging/KalkiLogTypes.h"
#include "KalkiCombatLogViewModel.generated.h"

class UKalkiLogSubsystem;

// Delegate for when combat log is updated
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCombatLogUpdated);

/**
 * ViewModel for the Combat Log display
 * Manages current combat messages and filtering
 * Transforms log messages into UI-friendly display entries
 */
UCLASS()
class KALKI_API UKalkiCombatLogViewModel : public UKalkiViewModelBase
{
    GENERATED_BODY()

public:
    // Lifecycle
    virtual void OnInitialize() override;
    virtual void OnShutdown() override;

    // Get combat messages for display
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    const TArray<FKalkiLogDisplayEntry>& GetCombatMessages() const { return CombatMessages; }

    // Channel filtering
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void SetChannelFilter(EKalkiLogChannel Channel, bool bShow);

    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    bool IsChannelFiltered(EKalkiLogChannel Channel) const;

    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void ClearAllFilters();

    // Combat session management
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void ClearCombatLog();

    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    int32 GetMessageCount() const { return CombatMessages.Num(); }

    // File operations
    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void OpenCurrentLogFile();

    UFUNCTION(BlueprintCallable, Category = "Kalki|CombatLog")
    void OpenLogFolder();

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Kalki|CombatLog")
    FOnCombatLogUpdated OnCombatLogUpdated;

protected:
    // Reference to log subsystem
    UPROPERTY()
    TObjectPtr<UKalkiLogSubsystem> LogSubsystem;

    // Current combat messages (display entries)
    UPROPERTY()
    TArray<FKalkiLogDisplayEntry> CombatMessages;

    // Current combat session ID we're tracking
    UPROPERTY()
    int32 TrackedCombatSessionID = -1;

    // Channel filters (true = show, false = hide)
    UPROPERTY()
    TMap<EKalkiLogChannel, bool> ChannelFilters;

    // Listen to new log messages
    UFUNCTION()
    void OnLogMessageAdded(const FKalkiLogMessage& Message);

    // Transform raw message to display entry
    FKalkiLogDisplayEntry TransformMessage(const FKalkiLogMessage& Message);

    // Extract interactive elements from message (FUTURE - placeholder for now)
    TArray<FKalkiLogInteractiveElement> ExtractInteractiveElements(const FKalkiLogMessage& Message);

    // Check if message passes filters
    bool PassesFilters(const FKalkiLogMessage& Message) const;

    // Refresh display from log subsystem
    void RefreshFromSubsystem();
};