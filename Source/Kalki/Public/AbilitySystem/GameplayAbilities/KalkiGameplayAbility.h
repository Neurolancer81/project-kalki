// Copyright of V.S. Puranam and no one else

// Public/AbilitySystem/GameplayAbilities/KalkiGameplayAbility.h

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "KalkiGameplayAbility.generated.h"

/**
 * Base class for all Kalki gameplay abilities
 * Handles ability execution, costs, cooldowns, and targeting
 */
UCLASS()
class KALKI_API UKalkiGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UKalkiGameplayAbility();

	// Custom functionality will be added here as needed
	// For now, this just inherits all standard GAS ability behavior
};