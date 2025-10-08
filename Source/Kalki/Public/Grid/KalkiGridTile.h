// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridTile.h

#pragma once

#include "CoreMinimal.h"
#include "KalkiTypes.h"
#include "GameFramework/Actor.h"
#include "KalkiGridTile.generated.h"

class AKalkiCharacter;
class UMaterialInstanceDynamic;

/**
 * Represents a single tile in the tactical grid
 * Can be occupied by a character and has movement/terrain properties
 */
UCLASS()
class KALKI_API AKalkiGridTile : public AActor
{
    GENERATED_BODY()

public:
    AKalkiGridTile();

    // Grid position
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    FIntPoint GetGridPosition() const { return GridPosition; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetGridPosition(const FIntPoint& Position);

    // Occupancy
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsOccupied() const { return OccupyingCharacter != nullptr; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    AKalkiCharacter* GetOccupyingCharacter() const { return OccupyingCharacter; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetOccupyingCharacter(AKalkiCharacter* KalkiCharacter);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void ClearOccupyingCharacter();

    // Movement cost
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    int32 GetMovementCost() const { return MovementCost; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetMovementCost(int32 Cost) { MovementCost = FMath::Max(1, Cost); }

    // Walkability
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsWalkable() const { return bIsWalkable; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool CanBeOccupied() const { return bIsWalkable && !IsOccupied(); }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetWalkable(bool bWalkable);

    // Visualization
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetHighlighted(bool bHighlight, FLinearColor Color = FLinearColor::Green);

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    bool IsHighlighted() const { return bIsHighlighted; }

    // Terrain Type - Getter and setter
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    EKalkiTerrainType GetTerrainType() const { return TerrainType; }

    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid")
    void SetTerrainType(EKalkiTerrainType Type);

protected:
    virtual void BeginPlay() override;
    // Register this tile with the grid manager
    void RegisterWithGridManager();
    virtual void OnConstruction(const FTransform& Transform) override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // Components
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> TileMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> TileRoot;

    // Grid data
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid")
    FIntPoint GridPosition;

    UPROPERTY()
    TObjectPtr<AKalkiCharacter> OccupyingCharacter;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
    int32 MovementCost = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
    bool bIsWalkable = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid")
    EKalkiTerrainType TerrainType = EKalkiTerrainType::Normal;

    // Visualization
    UPROPERTY()
    TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;

    UPROPERTY()
    bool bIsHighlighted = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Appearance")
    FLinearColor BaseColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.5f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Appearance")
    FLinearColor HighlightColor = FLinearColor::Green;

    // Helper to update material
    void UpdateMaterial();
};