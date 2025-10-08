// Copyright of V.S. Puranam and no one else

// Private/AbilitySystem/GameplayAbilities/KalkiGameplayAbility.cpp

#include "Kalki/Public/AbilitySystem/GameplayAbilities/KalkiGameplayAbility.h"

UKalkiGameplayAbility::UKalkiGameplayAbility()	
{
	// Default settings for Kalki abilities
    
	// Instancing policy - usually instanced per actor for RPGs
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    
	// Network settings - abilities execute on server, predict on client
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	NetSecurityPolicy = EGameplayAbilityNetSecurityPolicy::ServerOnly;
    
	// Default replication settings
	bReplicateInputDirectly = true;
}