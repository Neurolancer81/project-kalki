// Copyright of V.S. Puranam and no one else


#include "AbilitySystem/GameplayAbilities/KalkiGameplayAbility_MeleeAttack.h"

#include "AbilitySystem/KalkiAbilitySystemComponent.h"
#include "AbilitySystem/KalkiAttributeSet.h"
#include "AbilitySystem/GameplayEffects/KalkiGameplayEffect_Damage.h"
#include "Characters/KalkiCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "KalkiGameplayTags.h"


UKalkiGameplayAbility_MeleeAttack::UKalkiGameplayAbility_MeleeAttack()
{
	using namespace KalkiGameplayTags;

	// Set ability tags using native tags

	AbilityTags.AddTag(Ability_Attack_Melee);
	// Optionally set other tag containers:
	// ActivationOwnedTags - Tags granted to owner while ability is active
	// ActivationRequiredTags - Tags that must be present to activate
	// ActivationBlockedTags - Tags that prevent activation

	DamageEffectClass = UKalkiGameplayEffect_Damage::StaticClass();
	BaseDamage = 5.0f;
	AttackRange = 200.0f;
}

void UKalkiGameplayAbility_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
	}

	// Get the attacking character
	AKalkiCharacter* Attacker = Cast<AKalkiCharacter>(ActorInfo->AvatarActor.Get());
	if (!Attacker)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttack: No target in range"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// If attacker exists, find a target in range
	AKalkiCharacter* Target = FindTargetInRange(Attacker);
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("MeleeAttack: No target in range"));
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}

	// Calculate damage
	float DamageAmount = CalculateDamage(Attacker, Target);

	// Apply damage
	ApplyDamageToTarget(Target, DamageAmount);

	// Log the attack
	UE_LOG(LogTemp, Log, TEXT("%s attacks %s for %.0f damage"), 
		*Attacker->GetName(), *Target->GetName(), DamageAmount);

	// End the ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

AKalkiCharacter* UKalkiGameplayAbility_MeleeAttack::FindTargetInRange(const AKalkiCharacter* Attacker) const
{
	if (!Attacker || !Attacker->GetWorld()) return nullptr;

	// Simple targeting: find closest character within range
	// In a full implementation, this would use proper targeting system
    
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(Attacker->GetWorld(), AKalkiCharacter::StaticClass(), FoundActors);

	AKalkiCharacter* ClosestTarget = nullptr;
	float ClosestDistance = AttackRange;

	for (AActor* FoundActor : FoundActors)
	{
		AKalkiCharacter* PotentialTarget = Cast<AKalkiCharacter>(FoundActor);
		if (PotentialTarget == Attacker) continue;

		float Distance = FVector::Dist(PotentialTarget->GetActorLocation(), Attacker->GetActorLocation());

		if (Distance <= AttackRange && Distance < ClosestDistance)
		{
			ClosestTarget = PotentialTarget;
			ClosestDistance = Distance;
		}		
	}

	// Drawing Debug spheres
#if WITH_EDITORONLY_DATA
	DrawDebugSphere(Attacker->GetWorld(), Attacker->GetActorLocation(), AttackRange, 12, FColor::Red, false, 1.0f);
	if (ClosestTarget)
	{
		DrawDebugLine(Attacker->GetWorld(), Attacker->GetActorLocation(), 
			ClosestTarget->GetActorLocation(), FColor::Yellow, false, 1.0f, 0, 3.0f);
	}
#endif

	return ClosestTarget;	
}

float UKalkiGameplayAbility_MeleeAttack::CalculateDamage(const AKalkiCharacter* Attacker,
	const AKalkiCharacter* Target) const
{
	if (!Attacker || !Target)
	{
		return 0.0f;
	}

	// Get attacker's attribute set and ruleset
	UKalkiAttributeSet* AttackerAttributes = Attacker->GetAttributeSet();
	if (!AttackerAttributes)
	{
		return BaseDamage;
	}

	// For now, simple damage calculation: BaseDamage + Strength modifier
	// In full D&D implementation, this would include attack rolls, AC checks, etc.
    
	// Calculate Strength modifier (D&D formula: (Score - 10) / 2)
	float StrengthScore = AttackerAttributes->GetStrength();
	int32 StrengthMod = FMath::FloorToInt((StrengthScore - 10.0f) / 2.0f);

	float TotalDamage = BaseDamage + StrengthMod;

	// Minimum 1 damage
	return FMath::Max(1.0f, TotalDamage);	
}

void UKalkiGameplayAbility_MeleeAttack::ApplyDamageToTarget(AKalkiCharacter* Target, float DamageAmount)
{
	if (!Target || !DamageEffectClass) return;


	UKalkiAbilitySystemComponent* TargetASC = Cast<UKalkiAbilitySystemComponent>(Target->GetAbilitySystemComponent());
	if (!TargetASC) return;

	// Create effect context
	FGameplayEffectContextHandle EffectContext = TargetASC->MakeEffectContext();
	EffectContext.AddSourceObject(GetAvatarActorFromActorInfo());

	// Create effect spec
	FGameplayEffectSpecHandle SpecHandle = TargetASC->MakeOutgoingSpec(
		DamageEffectClass, 
		1.0f, // Level
		EffectContext
	);

	if (SpecHandle.IsValid())
	{
		// Set the damage amount via SetByCaller
		SpecHandle.Data->SetSetByCallerMagnitude(KalkiGameplayTags::Data_Damage, -DamageAmount);
		// Apply the effect
		TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());		
	}
}
