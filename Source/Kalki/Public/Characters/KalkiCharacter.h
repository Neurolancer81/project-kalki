// Public/Characters/KalkiCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Grid/Components/KalkiGridOccupant.h" // ✅ Implements interface
#include "KalkiTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "KalkiCharacter.generated.h"

class UKalkiDnD5eRuleset;
class UKalkiAttributeSet;
class UKalkiAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UKalkiGridOccupancyComponent; // ✅ Component

UCLASS()
class KALKI_API AKalkiCharacter : public ACharacter, 
                                   public IAbilitySystemInterface,
                                   public IKalkiGridOccupant // ✅ Implements interface
{
    GENERATED_BODY()

public:
    AKalkiCharacter(const FObjectInitializer& ObjectInitializer);

    // ========================================
    // IAbilitySystemInterface
    // ========================================

    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

    // Initialize the ability system
    virtual void InitializeAbilitySystem();

    // Getter for AS
    UKalkiAttributeSet* GetAttributeSet() const { return AttributeSet; }

    // ========================================
    // IKalkiGridOccupant Interface ✅ NEW
    // ========================================

    virtual FKalkiGridCoord GetGridPosition_Implementation() const override;
    virtual bool CanOccupyTile_Implementation(const FKalkiGridCoord& Coord) const override;
    virtual int32 GetOccupancySize_Implementation() const override;
    virtual UKalkiGridOccupancyComponent* GetGridOccupancyComponent_Implementation() const override;

    // ========================================
    // GRID HELPERS (Convenience)
    // ========================================

    /** Get movement range (from stats or default) */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
    int32 GetMovementRange() const;

    FKalkiCharacterData GetCharacterData() const {return CharacterData;}

    UKalkiDnD5eRuleset* GetActiveRuleSet() const {return ActiveRuleset;};

protected:
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;
    virtual void OnRep_PlayerState() override;

    // ========================================
    // COMPONENTS
    // ========================================

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UKalkiAbilitySystemComponent> AbilitySystemComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Abilities", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UKalkiAttributeSet> AttributeSet;

    /** Grid occupancy component ✅ NEW */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid", meta = (AllowPrivateAccess = "true"))
    TObjectPtr<UKalkiGridOccupancyComponent> GridOccupancyComponent;

    // ========================================
    // CHARACTER DATA
    // ========================================

    bool bAbilitySystemInitialized;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Character", meta = (AllowPrivateAccess = "true"))
    FKalkiCharacterData CharacterData;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kalki|Ruleset")
    TSubclassOf<UKalkiDnD5eRuleset> RulesetClass;

    UPROPERTY()
    TObjectPtr<UKalkiDnD5eRuleset> ActiveRuleset;

    /** Default movement range (tiles per turn) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
    int32 DefaultMovementRange = 6;

/********************************* START DEBUG STUFF ************************************************/
public:
    // Debug visualization
    UFUNCTION(BlueprintCallable, Category = "Kalki|Debug")
    void DrawDebugStats() const;

    // Or simpler - apply damage/heal directly through GAS
    void ApplyHealthChange(float Delta);

protected:
    // Tick to draw debug info
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Debug")
    bool bShowDebugStats = true;

/***************************** END DEBUG STUFF ************************************************/
};