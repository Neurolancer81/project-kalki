// Copyright of V.S. Puranam and no one else

// Public/Grid/KalkiGridTypes.h

#pragma once

#include "CoreMinimal.h"
#include "KalkiGridTypes.generated.h"

/**
 * Grid coordinate (2D integer position)
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiGridCoord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 X = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    int32 Y = 0;

    // Constructors
    FKalkiGridCoord() : X(0), Y(0) {}
    FKalkiGridCoord(int32 InX, int32 InY) : X(InX), Y(InY) {}

    // Operators
    bool operator==(const FKalkiGridCoord& Other) const
    {
        return X == Other.X && Y == Other.Y;
    }

    bool operator!=(const FKalkiGridCoord& Other) const
    {
        return !(*this == Other);
    }

    FKalkiGridCoord operator+(const FKalkiGridCoord& Other) const
    {
        return FKalkiGridCoord(X + Other.X, Y + Other.Y);
    }

    FKalkiGridCoord operator-(const FKalkiGridCoord& Other) const
    {
        return FKalkiGridCoord(X - Other.X, Y - Other.Y);
    }

    // Utility
    FString ToString() const
    {
        return FString::Printf(TEXT("(%d, %d)"), X, Y);
    }

    bool IsValid() const
    {
        return X >= 0 && Y >= 0;
    }

    // Manhattan distance
    int32 DistanceTo(const FKalkiGridCoord& Other) const
    {
        return FMath::Abs(X - Other.X) + FMath::Abs(Y - Other.Y);
    }

    // Chebyshev distance (diagonal allowed)
    int32 ChebyshevDistanceTo(const FKalkiGridCoord& Other) const
    {
        return FMath::Max(FMath::Abs(X - Other.X), FMath::Abs(Y - Other.Y));
    }

    // Have an Invalid value
    static FKalkiGridCoord Invalid()
    {
        return FKalkiGridCoord(-1, -1);
    }
};

// Hash function for using FKalkiGridCoord as TMap key
inline uint32 GetTypeHash(const FKalkiGridCoord& Coord)
{
    return HashCombine(GetTypeHash(Coord.X), GetTypeHash(Coord.Y));
}

/**
 * Terrain type for tiles
 */
UENUM(BlueprintType)
enum class EKalkiGridTerrainType : uint8
{
    Grass       UMETA(DisplayName = "Grass"),
    Stone       UMETA(DisplayName = "Stone"),
    Water       UMETA(DisplayName = "Water"),
    Lava        UMETA(DisplayName = "Lava"),
    Ice         UMETA(DisplayName = "Ice"),
    Dirt        UMETA(DisplayName = "Dirt"),
    Sand        UMETA(DisplayName = "Sand"),
    Wood        UMETA(DisplayName = "Wood (Floor)"),
    Void        UMETA(DisplayName = "Void (Invalid)")
};

/**
 * Cover type for tactical positioning
 */
UENUM(BlueprintType)
enum class EKalkiCoverType : uint8
{
    None        UMETA(DisplayName = "No Cover"),
    Half        UMETA(DisplayName = "Half Cover"),
    ThreeQuarter UMETA(DisplayName = "Three-Quarter Cover"),
    Full        UMETA(DisplayName = "Full Cover")
};

/**
 * Data for a single grid tile
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiGridTile
{
    GENERATED_BODY()

    /** Grid coordinates */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FKalkiGridCoord Coord;

    /** World position (center of tile, includes elevation) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    FVector WorldPosition = FVector::ZeroVector;

    /** Is this tile walkable? */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    bool bWalkable = true;

    /** Elevation (Z-axis height for line of sight and movement) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float Elevation = 0.0f;

    /** Terrain type */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    EKalkiGridTerrainType TerrainType = EKalkiGridTerrainType::Grass;

    /** Cover provided by this tile */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    EKalkiCoverType CoverType = EKalkiCoverType::None;

    /** Movement cost multiplier (1.0 = normal, 2.0 = difficult terrain) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Grid")
    float MovementCost = 1.0f;

    /** Actor occupying this tile (nullptr if empty) */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TWeakObjectPtr<AActor> Occupant;

    // Constructors
    FKalkiGridTile() {}
    FKalkiGridTile(const FKalkiGridCoord& InCoord, const FVector& InWorldPos)
        : Coord(InCoord), WorldPosition(InWorldPos) {}

    // Utility
    bool IsOccupied() const { return Occupant.IsValid(); }
    bool IsWalkable() const { return bWalkable && !IsOccupied(); }
};

/**
 * Path found by pathfinding
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiGridPath
{
    GENERATED_BODY()

    /** Ordered list of coordinates from start to end */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    TArray<FKalkiGridCoord> Waypoints;

    /** Total movement cost */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    float TotalCost = 0.0f;

    /** Is this a valid path? */
    UPROPERTY(BlueprintReadOnly, Category = "Grid")
    bool bIsValid = false;

    // Utility
    int32 GetLength() const { return Waypoints.Num(); }
    bool IsEmpty() const { return Waypoints.Num() == 0; }
    
    FKalkiGridCoord GetStart() const 
    { 
        return Waypoints.Num() > 0 ? Waypoints[0] : FKalkiGridCoord(); 
    }
    
    FKalkiGridCoord GetEnd() const 
    { 
        return Waypoints.Num() > 0 ? Waypoints.Last() : FKalkiGridCoord(); 
    }
};

/**
 * Dynamic grid change that needs to replicate
 * Used for runtime modifications (occupancy, destruction, etc.)
 * NOT USED YET - Reserved for future dynamic grid modifications
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiGridChange
{
    GENERATED_BODY()

    UPROPERTY()
    FKalkiGridCoord Coord;

    UPROPERTY()
    bool bWalkable = true;

    UPROPERTY()
    float Elevation = 0.0f;

    UPROPERTY()
    TWeakObjectPtr<AActor> Occupant;

    UPROPERTY()
    EKalkiGridTerrainType GridTerrainType = EKalkiGridTerrainType::Grass;

    FKalkiGridChange() {}
};

/**
 * Movement Tier
 * Defines a range tier with its visual representation
 * Used for showing Normal Move, Dash, Triple Move, etc.
 * 
 * EXAMPLES:
 * - D&D 5e: Normal (6 tiles, cyan), Dash (12 tiles, yellow)
 * - PF2e: 1-Action (6), 2-Action (12), 3-Action (18)
 * - Special: Flying (15 tiles, blue), Climbing (8 tiles, green)
 */
USTRUCT(BlueprintType)
struct KALKI_API FKalkiMovementTier
{
    GENERATED_BODY()

    /** Maximum range for this tier (in tiles) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    int32 Range = 0;

    /** Border color for this tier */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FLinearColor BorderColor = FLinearColor::White;

    /** Display name (for debugging and UI) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    FString TierName = TEXT("Unknown");

    /** Border thickness (in world units) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BorderThickness = 3.0f;

    /** Z-height offset (for layering) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ZOffset = 5.0f;

    // ========================================
    // CONSTRUCTORS
    // ========================================

    /** Default constructor */
    FKalkiMovementTier()
        : Range(0)
        , BorderColor(FLinearColor::White)
        , TierName(TEXT("Unknown"))
        , BorderThickness(3.0f)
        , ZOffset(5.0f)
    {
    }

    /** Convenience constructor */
    FKalkiMovementTier(int32 InRange, const FLinearColor& InColor, const FString& InName, float InZOffset = 5.0f)
        : Range(InRange)
        , BorderColor(InColor)
        , TierName(InName)
        , BorderThickness(3.0f)
        , ZOffset(InZOffset)
    {
    }

    // ========================================
    // UTILITY
    // ========================================

    /** Check if tier is valid */
    bool IsValid() const
    {
        return Range > 0;
    }

    /** Debug string */
    FString ToString() const
    {
        return FString::Printf(TEXT("%s: %d tiles (R:%.2f G:%.2f B:%.2f)"),
            *TierName,
            Range,
            BorderColor.R,
            BorderColor.G,
            BorderColor.B
        );
    }

    /** Comparison (for sorting by range) */
    bool operator<(const FKalkiMovementTier& Other) const
    {
        return Range < Other.Range;
    }

    bool operator==(const FKalkiMovementTier& Other) const
    {
        return Range == Other.Range && TierName.Equals(Other.TierName);
    }
};