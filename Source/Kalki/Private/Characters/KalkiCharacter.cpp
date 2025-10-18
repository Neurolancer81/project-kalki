// Private/Characters/KalkiCharacter.cpp

#include "Characters/KalkiCharacter.h"
#include "AbilitySystem/KalkiAbilitySystemComponent.h"
#include "AbilitySystem/KalkiAttributeSet.h"
#include "Ruleset/KalkiDnD5eRuleset.h"
#include "Grid/Components/KalkiGridOccupancyComponent.h" // ✅ ADD
#include "Logging/KalkiLog.h" // ✅ ADD
#include "DrawDebugHelpers.h"

AKalkiCharacter::AKalkiCharacter(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , bAbilitySystemInitialized(false)
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Create ability system component
    AbilitySystemComponent = CreateDefaultSubobject<UKalkiAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
    AbilitySystemComponent->SetIsReplicated(true);
    AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

    // Create attribute set
    AttributeSet = CreateDefaultSubobject<UKalkiAttributeSet>(TEXT("AttributeSet"));

    // ✅ CREATE GRID OCCUPANCY COMPONENT
    GridOccupancyComponent = CreateDefaultSubobject<UKalkiGridOccupancyComponent>(TEXT("GridOccupancyComponent"));
}

// Override for the IASC
UAbilitySystemComponent* AKalkiCharacter::GetAbilitySystemComponent() const
{
    return AbilitySystemComponent;
}

void AKalkiCharacter::BeginPlay()
{
    Super::BeginPlay();
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
// IKalkiGridOccupant Interface ✅ NEW
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

/************************************ DEBUG ********************************************************/

void AKalkiCharacter::DrawDebugStats() const
{
    if (!AttributeSet)
    {
        return;
    }

    FVector Location = GetActorLocation() + FVector(0, 0, 100); // Above character

    // ✅ UPDATED - Include grid position
    FString DebugText = FString::Printf(
        TEXT("%s (Lvl %d %s)\n")
        TEXT("Grid: %s\n") // ✅ ADD
        TEXT("STR: %.0f (%+d)  DEX: %.0f (%+d)  CON: %.0f (%+d)\n")
        TEXT("INT: %.0f (%+d)  WIS: %.0f (%+d)  CHA: %.0f (%+d)\n")
        TEXT("HP: %.0f/%.0f  AC: %.0f  Move: %d"), // ✅ ADD Move
        *CharacterData.CharacterName,
        CharacterData.Level,
        *UEnum::GetValueAsString(CharacterData.Class),
        *IKalkiGridOccupant::Execute_GetGridPosition(this).ToString(), // ✅ Use interface
        AttributeSet->GetStrength(),
        ActiveRuleset ? ActiveRuleset->CalculateAbilityModifier(AttributeSet->GetStrength()) : 0,
        AttributeSet->GetDexterity(),
        ActiveRuleset ? ActiveRuleset->CalculateAbilityModifier(AttributeSet->GetDexterity()) : 0,
        AttributeSet->GetConstitution(),
        ActiveRuleset ? ActiveRuleset->CalculateAbilityModifier(AttributeSet->GetConstitution()) : 0,
        AttributeSet->GetIntelligence(),
        ActiveRuleset ? ActiveRuleset->CalculateAbilityModifier(AttributeSet->GetIntelligence()) : 0,
        AttributeSet->GetWisdom(),
        ActiveRuleset ? ActiveRuleset->CalculateAbilityModifier(AttributeSet->GetWisdom()) : 0,
        AttributeSet->GetCharisma(),
        ActiveRuleset ? ActiveRuleset->CalculateAbilityModifier(AttributeSet->GetCharisma()) : 0,
        AttributeSet->GetHealth(),
        AttributeSet->GetMaxHealth(),
        AttributeSet->GetArmorClass(),
        GetMovementRange() // ✅ ADD
    );

    DrawDebugString(GetWorld(), Location, DebugText, nullptr, FColor::White, 0.0f, true, 1.2f);
}

void AKalkiCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bShowDebugStats)
    {
        DrawDebugStats();
    }
}

void AKalkiCharacter::ApplyHealthChange(float Delta)
{
    if (!AbilitySystemComponent || !AttributeSet)
    {
        return;
    }

    const float CurrentHealth = AttributeSet->GetHealth();
    const float MaxHealth = AttributeSet->GetMaxHealth();
    const float NewHealth = FMath::Clamp(CurrentHealth + Delta, 0.0f, MaxHealth);

    AbilitySystemComponent->SetNumericAttributeBase(
        UKalkiAttributeSet::GetHealthAttribute(),
        NewHealth
    );
}

/*********************************************** END DEBUG *************************/