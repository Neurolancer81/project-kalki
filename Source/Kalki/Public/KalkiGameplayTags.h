// Public/KalkiGameplayTags.h

#pragma once

#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

/**
 * Kalki Gameplay Tags
 * Native gameplay tags defined in C++ for compile-time safety
 */
namespace KalkiGameplayTags
{
	// Ability Tags
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Melee);
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Attack_Ranged);
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Skill);
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Spell);

	// Effect Tags
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Damage);
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Effect_Heal);

	// State Tags
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Dead);
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(State_Stunned);

	// Data Tags (for SetByCaller)
	KALKI_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
}