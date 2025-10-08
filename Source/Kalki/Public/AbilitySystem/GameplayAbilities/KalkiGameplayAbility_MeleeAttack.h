// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "KalkiGameplayAbility.h"
#include "KalkiGameplayAbility_MeleeAttack.generated.h"

class UKalkiGameplayEffect_Damage;
class AKalkiCharacter;
/**
 * 
 */
UCLASS()
class KALKI_API UKalkiGameplayAbility_MeleeAttack : public UKalkiGameplayAbility
{
	GENERATED_BODY()

public:
	UKalkiGameplayAbility_MeleeAttack();

	// Ability Execution
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

protected:
	// Damage effect to apply
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kalki|Damage")
	TSubclassOf<UKalkiGameplayEffect_Damage> DamageEffectClass;

	// Base damage (before ability modifiers)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kalki|Damage")
	float BaseDamage = 5.0f;

	// Maximum attack range
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kalki|Targeting")
	float AttackRange = 200.0f;

	// Helper functions
	AKalkiCharacter* FindTargetInRange(const AKalkiCharacter* Attacker) const;
	float CalculateDamage(const AKalkiCharacter* Attacker, const AKalkiCharacter* Target) const;
	void ApplyDamageToTarget(AKalkiCharacter* Target, float DamageAmount);
	
};
