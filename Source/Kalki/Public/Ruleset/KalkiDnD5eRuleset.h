// Copyright of V.S. Puranam and no one else

// Public/Ruleset/KalkiDnD5eRuleset.h

#pragma once

#include "CoreMinimal.h"
#include "Ruleset/KalkiRulesetProvider.h"
#include "KalkiDnD5eRuleset.generated.h"

/**
 * D&D 5.2 SRD Ruleset Implementation
 * Handles character creation, stat calculation, and game rules for D&D 5e
 */
UCLASS(Blueprintable)
class KALKI_API UKalkiDnD5eRuleset : public UObject, public IKalkiRulesetProvider
{
	GENERATED_BODY()

public:
	UKalkiDnD5eRuleset();

	// IKalkiRulesetProvider interface
	virtual void InitializeCharacterAttributes(UKalkiAttributeSet* AttributeSet, const FKalkiCharacterData& CharData) override;
	virtual void CalculateDerivedStats(UKalkiAttributeSet* AttributeSet) override;
	virtual int32 CalculateAttackBonus(const UKalkiAttributeSet* AttributeSet, EKalkiDamageType DamageType) const override;
	virtual int32 CalculateAbilityModifier(float AbilityScore) const override;

	// D&D 5e specific calculations
    
	/** Calculate proficiency bonus based on character level */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Ruleset")
	int32 CalculateProficiencyBonus(int32 Level) const;

	/** Calculate max hit points based on class, level, and Constitution */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Ruleset")
	float CalculateMaxHitPoints(EKalkiCharacterClass Class, int32 Level, int32 ConstitutionModifier) const;

	/** Calculate armor class (base 10 + Dex mod for unarmored) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Ruleset")
	float CalculateArmorClass(int32 DexterityModifier, float ArmorBonus = 0.0f, float ShieldBonus = 0.0f) const;

	/** Calculate saving throw modifier */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Ruleset")
	int32 CalculateSavingThrow(float AbilityScore, int32 ProficiencyBonus, bool bIsProficient) const;

protected:
	/** Get hit die size for a character class (d6, d8, d10, d12) */
	int32 GetHitDieForClass(EKalkiCharacterClass Class) const;
};