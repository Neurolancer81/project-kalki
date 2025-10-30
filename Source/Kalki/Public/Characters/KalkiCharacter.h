// Public/Characters/KalkiCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Grid/Components/KalkiGridOccupant.h"
#include "KalkiTypes.h"

#include "KalkiCharacter.generated.h"

class UKalkiDnD5eRuleset;
class UKalkiAttributeSet;
class UKalkiAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;
class UKalkiGridOccupancyComponent;

UCLASS()
class KALKI_API AKalkiCharacter : public ACharacter, 
                                   public IAbilitySystemInterface,
                                   public IKalkiGridOccupant
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
    // IKalkiGridOccupant Interface
    // ========================================

    virtual FKalkiGridCoord GetGridPosition_Implementation() const override;
    virtual bool CanOccupyTile_Implementation(const FKalkiGridCoord& Coord) const override;
    virtual int32 GetOccupancySize_Implementation() const override;
    virtual UKalkiGridOccupancyComponent* GetGridOccupancyComponent_Implementation() const override;

    // ========================================
    // GRID HELPERS
    // ========================================

    /** Get movement range (from stats or default) */
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid")
    int32 GetMovementRange() const;

    /** Force snap character to nearest grid tile */
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool SnapToNearestTile();

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

    /** Grid occupancy component */
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

    /** Auto-place on grid at BeginPlay? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
    bool bAutoPlaceOnGrid = true;

private:
    // ========================================
    // INTERNAL GRID FUNCTIONS
    // ========================================

    /** Initialize grid placement at BeginPlay */
    void InitializeGridPlacement();

    /** Place character on grid at nearest valid tile */
    bool PlaceOnNearestTile();

    /** Find nearby empty tile if placement fails */
    FKalkiGridCoord FindNearbyEmptyTile(const FKalkiGridCoord& StartCoord, int32 MaxRadius = 5);

    /** Called when grid is created */
    UFUNCTION()
    void OnGridCreated();

    /** Called when grid is cleared */
    UFUNCTION()
    void OnGridCleared();

    /** Track if we attempted placement */
    bool bAttemptedGridPlacement = false;

    /** Deferred grid initialization (called next frame) */
    void DeferredGridInitialization();
};


