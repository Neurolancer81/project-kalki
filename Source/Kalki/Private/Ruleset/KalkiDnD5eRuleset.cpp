// Copyright of V.S. Puranam and no one else

// Private/Ruleset/KalkiDnD5eRuleset.cpp

#include "Ruleset/KalkiDnD5eRuleset.h"
#include "AbilitySystem/KalkiAttributeSet.h"

UKalkiDnD5eRuleset::UKalkiDnD5eRuleset()
{
}

void UKalkiDnD5eRuleset::InitializeCharacterAttributes(UKalkiAttributeSet* AttributeSet, const FKalkiCharacterData& CharData)
{
    if (!AttributeSet)
    {
        UE_LOG(LogTemp, Warning, TEXT("UKalkiDnD5eRuleset::InitializeCharacterAttributes - AttributeSet is null"));
        return;
    }

    // Set base ability scores from character data
    // Use FindRef which returns 0 if key not found, or provide default value
    const float DefaultAbilityScore = 10.0f;
    
    AttributeSet->SetStrength(CharData.BaseAttributes.Contains(TEXT("Strength")) 
        ? CharData.BaseAttributes[TEXT("Strength")] 
        : DefaultAbilityScore);
    
    AttributeSet->SetDexterity(CharData.BaseAttributes.Contains(TEXT("Dexterity")) 
        ? CharData.BaseAttributes[TEXT("Dexterity")] 
        : DefaultAbilityScore);
    
    AttributeSet->SetConstitution(CharData.BaseAttributes.Contains(TEXT("Constitution")) 
        ? CharData.BaseAttributes[TEXT("Constitution")] 
        : DefaultAbilityScore);
    
    AttributeSet->SetIntelligence(CharData.BaseAttributes.Contains(TEXT("Intelligence")) 
        ? CharData.BaseAttributes[TEXT("Intelligence")] 
        : DefaultAbilityScore);
    
    AttributeSet->SetWisdom(CharData.BaseAttributes.Contains(TEXT("Wisdom")) 
        ? CharData.BaseAttributes[TEXT("Wisdom")] 
        : DefaultAbilityScore);
    
    AttributeSet->SetCharisma(CharData.BaseAttributes.Contains(TEXT("Charisma")) 
        ? CharData.BaseAttributes[TEXT("Charisma")] 
        : DefaultAbilityScore);

    // Calculate and set derived stats
    CalculateDerivedStats(AttributeSet);
}

void UKalkiDnD5eRuleset::CalculateDerivedStats(UKalkiAttributeSet* AttributeSet)
{
    if (!AttributeSet)
    {
        return;
    }

    // For now, assume level 1 character - later this will come from character data
    const int32 Level = 1;
    const EKalkiCharacterClass Class = EKalkiCharacterClass::Fighter; // Default for now

    // Calculate ability modifiers
    const int32 ConMod = CalculateAbilityModifier(AttributeSet->GetConstitution());
    const int32 DexMod = CalculateAbilityModifier(AttributeSet->GetDexterity());

    // Calculate max HP based on class and Constitution
    const float MaxHP = CalculateMaxHitPoints(Class, Level, ConMod);
    AttributeSet->SetMaxHealth(MaxHP);
    AttributeSet->SetHealth(MaxHP); // Start at full health

    // Calculate AC (base 10 + Dex modifier for unarmored)
    const float AC = CalculateArmorClass(DexMod);
    AttributeSet->SetArmorClass(AC);

    // TODO: Calculate other derived stats as needed
    // - Initiative bonus (Dex mod)
    // - Saving throw proficiencies
    // - Skill bonuses
}

int32 UKalkiDnD5eRuleset::CalculateAttackBonus(const UKalkiAttributeSet* AttributeSet, EKalkiDamageType DamageType) const
{
    if (!AttributeSet)
    {
        return 0;
    }

    // For now, simple implementation
    // Melee attacks use Strength, others could use Dexterity
    // In full implementation, this would depend on weapon properties
    
    int32 RelevantAbilityMod = 0;
    
    switch (DamageType)
    {
        case EKalkiDamageType::Physical:
            // Use higher of Strength or Dexterity (finesse weapon consideration)
            RelevantAbilityMod = FMath::Max(
                CalculateAbilityModifier(AttributeSet->GetStrength()),
                CalculateAbilityModifier(AttributeSet->GetDexterity())
            );
            break;
        
        default:
            // For spell attacks, might use Int/Wis/Cha
            RelevantAbilityMod = CalculateAbilityModifier(AttributeSet->GetIntelligence());
            break;
    }

    // Assume level 1 for now
    const int32 ProficiencyBonus = CalculateProficiencyBonus(1);

    return RelevantAbilityMod + ProficiencyBonus;
}

int32 UKalkiDnD5eRuleset::CalculateAbilityModifier(float AbilityScore) const
{
    // D&D 5e formula: (Score - 10) / 2, rounded down
    return FMath::FloorToInt((AbilityScore - 10.0f) / 2.0f);
}

int32 UKalkiDnD5eRuleset::CalculateProficiencyBonus(int32 Level) const
{
    // D&D 5e proficiency bonus by level
    // Levels 1-4: +2
    // Levels 5-8: +3
    // Levels 9-12: +4
    // Levels 13-16: +5
    // Levels 17-20: +6
    
    if (Level <= 0)
    {
        return 0;
    }
    
    return 2 + (Level - 1) / 4;
}

float UKalkiDnD5eRuleset::CalculateMaxHitPoints(EKalkiCharacterClass Class, int32 Level, int32 ConstitutionModifier) const
{
    if (Level <= 0)
    {
        return 1.0f;
    }

    const int32 HitDie = GetHitDieForClass(Class);
    
    // First level: Max hit die + Con mod
    float MaxHP = HitDie + ConstitutionModifier;
    
    // Subsequent levels: Average of hit die (rounded up) + Con mod
    // For simplicity, using (HitDie / 2 + 1) as average
    if (Level > 1)
    {
        const int32 AverageHitDieRoll = (HitDie / 2) + 1;
        MaxHP += (Level - 1) * (AverageHitDieRoll + ConstitutionModifier);
    }
    
    // Minimum 1 HP per level
    MaxHP = FMath::Max(MaxHP, static_cast<float>(Level));
    
    return MaxHP;
}

float UKalkiDnD5eRuleset::CalculateArmorClass(int32 DexterityModifier, float ArmorBonus, float ShieldBonus) const
{
    // Base AC is 10 + Dex modifier (unarmored)
    // ArmorBonus and ShieldBonus can be added when equipment is implemented
    return 10.0f + DexterityModifier + ArmorBonus + ShieldBonus;
}

int32 UKalkiDnD5eRuleset::CalculateSavingThrow(float AbilityScore, int32 ProficiencyBonus, bool bIsProficient) const
{
    int32 AbilityMod = CalculateAbilityModifier(AbilityScore);
    
    if (bIsProficient)
    {
        return AbilityMod + ProficiencyBonus;
    }
    
    return AbilityMod;
}

int32 UKalkiDnD5eRuleset::GetHitDieForClass(EKalkiCharacterClass Class) const
{
    // D&D 5e hit dice by class
    switch (Class)
    {
        case EKalkiCharacterClass::Fighter:
            return 10; // d10
        
        case EKalkiCharacterClass::Mage:
            return 6;  // d6
        
        case EKalkiCharacterClass::Cleric:
            return 8;  // d8
        
        case EKalkiCharacterClass::Rogue:
            return 8;  // d8
        
        case EKalkiCharacterClass::None:
        default:
            return 8;  // d8 as default
    }
}