// Private/Characters/KalkiCharacter.cpp

#include "Characters/KalkiCharacter.h"
#include "AbilitySystem/KalkiAbilitySystemComponent.h"
#include "AbilitySystem/KalkiAttributeSet.h"
#include "Ruleset/KalkiDnD5eRuleset.h"
#include "Grid/Components/KalkiGridOccupancyComponent.h"
#include "Grid/KalkiGridManager.h"
#include "Logging/KalkiLog.h"

AKalkiCharacter::AKalkiCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bAbilitySystemInitialized(false)
{
    PrimaryActorTick.bCanEverTick = false;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Create ability system component
    AbilitySystemComponent = CreateDefaultSubobject<UKalkiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create attribute set
    AttributeSet = CreateDefaultSubobject<UKalkiAttributeSet>(TEXT("AttributeSet"));

    // Create grid occupancy component
    GridOccupancyComponent = CreateDefaultSubobject<UKalkiGridOccupancyComponent>(TEXT("GridOccupancyComponent"));

    // Defer grid initialization to next tick
    // This ensures all subsystems are initialized
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DeferredGridInitialization);
    }
}

// Override for the IASC
UAbilitySystemComponent* AKalkiCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Initialize grid placement
    InitializeGridPlacement();
}

void AKalkiCharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    // Server-side initialization
    if (AbilitySystemComponent && !bAbilitySystemInitialized)
    {
        InitializeAbilitySystem();
    }
}

void AKalkiCharacter::OnRep_PlayerState()
{
    Super::OnRep_PlayerState();

    // Client-side initialization
    if (AbilitySystemComponent && !bAbilitySystemInitialized)
    {
        InitializeAbilitySystem();
    }
}

void AKalkiCharacter::InitializeAbilitySystem()
{
    if (bAbilitySystemInitialized)
    {
        return;
    }

    if (!AbilitySystemComponent || !AttributeSet)
    {
        UE_LOG(LogTemp, Error, TEXT("AKalkiCharacter::InitializeAbilitySystem - Missing ASC or AttributeSet"));
        return;
    }

    // Initialize GAS
    AbilitySystemComponent->InitAbilityActorInfo(this, this);

    // Create ruleset instance
    if (RulesetClass)
    {
        ActiveRuleset = NewObject<UKalkiDnD5eRuleset>(this, RulesetClass);
    }
    else
    {
        ActiveRuleset = NewObject<UKalkiDnD5eRuleset>(this);
    }

    // Validate character data
    if (CharacterData.CharacterName.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Character spawned without CharacterData! Using defaults."));
        CharacterData.CharacterName = TEXT("Unnamed Character");
        CharacterData.Class = EKalkiCharacterClass::Fighter;
        CharacterData.Level = 1;
        CharacterData.BaseAttributes.Add(TEXT("Strength"), 10.0f);
        CharacterData.BaseAttributes.Add(TEXT("Dexterity"), 10.0f);
        CharacterData.BaseAttributes.Add(TEXT("Constitution"), 10.0f);
        CharacterData.BaseAttributes.Add(TEXT("Intelligence"), 10.0f);
        CharacterData.BaseAttributes.Add(TEXT("Wisdom"), 10.0f);
        CharacterData.BaseAttributes.Add(TEXT("Charisma"), 10.0f);
    }

    // Initialize attributes using ruleset
    ActiveRuleset->InitializeCharacterAttributes(AttributeSet, CharacterData);

    // Grant starting abilities
    if (CharacterData.StartingAbilities.Num() > 0)
    {
        for (const TSubclassOf<UGameplayAbility>& AbilityClass : CharacterData.StartingAbilities)
        {
            if (AbilityClass)
            {
                FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,
                    1.f, INDEX_NONE, nullptr);
                AbilitySystemComponent->GiveAbility(AbilitySpec);
                UE_LOG(LogTemp, Log, TEXT("Granted ability: %s to %s"),
                    *AbilityClass->GetName(), *CharacterData.CharacterName);
            }
        }
    }

    bAbilitySystemInitialized = true;

    UE_LOG(LogTemp, Log, TEXT("Initialized character: %s (Level %d %s)"),
        *CharacterData.CharacterName,
        CharacterData.Level,
        *UEnum::GetValueAsString(CharacterData.Class));
}

// ========================================
// IKalkiGridOccupant Interface
// ========================================

FKalkiGridCoord AKalkiCharacter::GetGridPosition_Implementation() const
{
    if (GridOccupancyComponent)
    {
        return GridOccupancyComponent->GetGridPosition();
    }
    return FKalkiGridCoord::Invalid();
}

bool AKalkiCharacter::CanOccupyTile_Implementation(const FKalkiGridCoord& Coord) const
{
    if (GridOccupancyComponent)
    {
        return GridOccupancyComponent->CanOccupyTile(Coord);
    }
    return false;
}

int32 AKalkiCharacter::GetOccupancySize_Implementation() const
{
    if (GridOccupancyComponent)
    {
        return GridOccupancyComponent->OccupancySize;
    }
    return 1;
}

UKalkiGridOccupancyComponent* AKalkiCharacter::GetGridOccupancyComponent_Implementation() const
{
    return GridOccupancyComponent;
}

// ========================================
// GRID HELPERS
// ========================================

int32 AKalkiCharacter::GetMovementRange() const
{
    // TODO: Calculate from stats
    // - Base movement from race
    // - Dexterity modifier
    // - Class features (Monk speed, etc.)
    // - Temporary effects (Haste, Slow, etc.)
    
    // For now, return default
    return DefaultMovementRange;
}

bool AKalkiCharacter::SnapToNearestTile()
{
    return PlaceOnNearestTile();
}

// ========================================
// INTERNAL GRID FUNCTIONS
// ========================================

void AKalkiCharacter::InitializeGridPlacement()
{
    // Skip if auto-place disabled
    if (!bAutoPlaceOnGrid)
    {
        return;
    }

    // Skip if already on grid
    if (GridOccupancyComponent && GridOccupancyComponent->IsOnGrid())
    {
        return;
    }

    // Skip if already attempted
    if (bAttemptedGridPlacement)
    {
        return;
    }

    // Mark as attempted
    bAttemptedGridPlacement = true;

    // Attempt to place on grid
    PlaceOnNearestTile();
}

// Delegate handler implementations
void AKalkiCharacter::OnGridCreated()
{
    // Grid was just created - try to place character
    InitializeGridPlacement();
}

void AKalkiCharacter::OnGridCleared()
{
    // Grid was cleared - remove from grid
    if (GridOccupancyComponent && GridOccupancyComponent->IsOnGrid())
    {
        GridOccupancyComponent->RemoveFromGrid();
    }

    // Reset placement flag so we can try again when grid is recreated
    bAttemptedGridPlacement = false;
}

bool AKalkiCharacter::PlaceOnNearestTile()
{
    if (!GridOccupancyComponent)
    {
        KalkiLog::Grid(
            TEXT("PlaceOnNearestTile - No GridOccupancyComponent"),
            EKalkiLogSeverity::Error,
            this
        );
        return false;
    }

    // Get grid manager
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
 
    // Get current world position
    FVector CurrentWorldPos = GetActorLocation();
    
    // Convert to grid coordinate (rounds to nearest tile)
    FKalkiGridCoord NearestCoord = GridManager->WorldPositionToCoord(CurrentWorldPos);
    
    // Validate coordinate
    if (!GridManager->IsValidCoord(NearestCoord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("%s outside grid bounds at world position: (%.0f, %.0f, %.0f)"),
                *GetName(),
                CurrentWorldPos.X, CurrentWorldPos.Y, CurrentWorldPos.Z),
            EKalkiLogSeverity::Warning,
            this
        );
        return false;
    }

    // Remove from current position if already on grid
    if (GridOccupancyComponent->IsOnGrid())
    {
        GridOccupancyComponent->RemoveFromGrid();
    }

    // Try to place on nearest tile
    if (GridOccupancyComponent->PlaceOnGrid(NearestCoord))
    {
        FVector SnappedWorldPos = GridManager->CoordToWorldPosition(NearestCoord);
        
        KalkiLog::Grid(
            FString::Printf(TEXT("%s placed on grid at: %s (World: %.0f, %.0f, %.0f)"),
                *GetName(),
                *NearestCoord.ToString(),
                SnappedWorldPos.X, SnappedWorldPos.Y, SnappedWorldPos.Z),
            EKalkiLogSeverity::Verbose,
            this
        );
        
        return true;
    }

    // Tile occupied or unwalkable - try to find nearby empty tile
    FKalkiGridCoord EmptyTile = FindNearbyEmptyTile(NearestCoord);
    
    if (EmptyTile.IsValid() && GridOccupancyComponent->PlaceOnGrid(EmptyTile))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("%s placed on nearby tile: %s (original %s was occupied)"),
                *GetName(),
                *EmptyTile.ToString(),
                *NearestCoord.ToString()),
            EKalkiLogSeverity::Verbose,
            this
        );
        
        return true;
    }

    // Failed to place
    KalkiLog::Grid(
        FString::Printf(TEXT("%s failed to find empty tile near: %s"),
            *GetName(),
            *NearestCoord.ToString()),
        EKalkiLogSeverity::Error,
        this
    );
    
    return false;
}

FKalkiGridCoord AKalkiCharacter::FindNearbyEmptyTile(const FKalkiGridCoord& StartCoord, int32 MaxRadius)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        return FKalkiGridCoord::Invalid();
    }

    // Search in expanding rings around the start coordinate
    for (int32 Radius = 1; Radius <= MaxRadius; ++Radius)
    {
        // Check tiles in a square ring at this radius
        for (int32 X = StartCoord.X - Radius; X <= StartCoord.X + Radius; ++X)
        {
            for (int32 Y = StartCoord.Y - Radius; Y <= StartCoord.Y + Radius; ++Y)
            {
                // Only check perimeter (not interior)
                bool bIsPerimeter = (FMath::Abs(X - StartCoord.X) == Radius) || 
                                   (FMath::Abs(Y - StartCoord.Y) == Radius);
                
                if (!bIsPerimeter)
                {
                    continue;
                }

                FKalkiGridCoord TestCoord(X, Y);
                
                // Check if valid
                if (!GridManager->IsValidCoord(TestCoord))
                {
                    continue;
                }

                // Check if tile is empty and walkable
                FKalkiGridTile Tile = GridManager->GetTile(TestCoord);
                if (Tile.bWalkable && !Tile.IsOccupied())
                {
                    return TestCoord;
                }
            }
        }
    }

    // No empty tile found
    return FKalkiGridCoord::Invalid();
}

void AKalkiCharacter::DeferredGridInitialization()
{
    // Safe to access subsystems now
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    UKalkiGridManager* GridManager = World->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("KalkiCharacter - GridManager subsystem not found for %s"), *GetName());
        return;
    }

    // Bind to grid events (remove first to prevent duplicates)
    GridManager->OnGridCreated.RemoveDynamic(this, &AKalkiCharacter::OnGridCreated);
    GridManager->OnGridCreated.AddDynamic(this, &AKalkiCharacter::OnGridCreated);

    GridManager->OnGridCleared.RemoveDynamic(this, &AKalkiCharacter::OnGridCleared);
    GridManager->OnGridCleared.AddDynamic(this, &AKalkiCharacter::OnGridCleared);

    // Try immediate placement
    InitializeGridPlacement();
}


