// Private/KalkiGameplayTags.cpp

#include "KalkiGameplayTags.h"

namespace KalkiGameplayTags
{
	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack_Melee, "Ability.Attack.Melee", "Basic melee attack ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Attack_Ranged, "Ability.Attack.Ranged", "Ranged attack ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Skill, "Ability.Skill", "Character skill ability");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Ability_Spell, "Ability.Spell", "Spell ability");

	// Effect Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_Damage, "Effect.Damage", "Damage effect tag");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Effect_Heal, "Effect.Heal", "Healing effect tag");

	// State Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Dead, "State.Dead", "Character is dead");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(State_Stunned, "State.Stunned", "Character is stunned");

	// Data Tags
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Data_Damage, "Data.Damage", "SetByCaller tag for damage amount");
}