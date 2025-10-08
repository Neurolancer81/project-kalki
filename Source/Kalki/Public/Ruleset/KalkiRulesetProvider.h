// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "KalkiTypes.h"
#include "UObject/Interface.h"
#include "KalkiRulesetProvider.generated.h"


class UKalkiAttributeSet;
// This class does not need to be modified.
UINTERFACE()
class UKalkiRulesetProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class KALKI_API IKalkiRulesetProvider
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Initialize a character's attributes based on this ruleset and character data
	virtual void InitializeCharacterAttributes(UKalkiAttributeSet* AttributeSet, const FKalkiCharacterData& CharData) = 0;

	// Calculate derived stats (AC, HP, etc.) from base attributes
	virtual void CalculateDerivedStats(UKalkiAttributeSet* AttributeSet) = 0;

	// Calculate attack roll (returns total modifier, dice rolls happen elsewhere)
	virtual int32 CalculateAttackBonus(const UKalkiAttributeSet* AttributeSet, EKalkiDamageType DamageType) const = 0;

	// Calculate ability modifier from score
	virtual int32 CalculateAbilityModifier(float AbilityScore) const = 0;

	// More methods as needed
};
