// Copyright of V.S. Puranam and no one else


#include "AbilitySystem/GameplayEffects/KalkiGameplayEffect_Damage.h"
#include "AbilitySystem/KalkiAttributeSet.h"
#include "KalkiGameplayTags.h"
#include "GameplayEffectComponents/AssetTagsGameplayEffectComponent.h"

class UAssetTagsGameplayEffectComponent;

UKalkiGameplayEffect_Damage::UKalkiGameplayEffect_Damage()
{
	using namespace KalkiGameplayTags;

	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute = UKalkiAttributeSet::GetHealthAttribute();
	HealthModifier.ModifierOp = EGameplayModOp::Additive;
    
	FSetByCallerFloat DamageMagnitude;
	DamageMagnitude.DataTag = Data_Damage; // Using native tag
    
	FGameplayEffectModifierMagnitude ModifierMagnitude(DamageMagnitude);
	HealthModifier.ModifierMagnitude = ModifierMagnitude;

	Modifiers.Add(HealthModifier);
	
}
