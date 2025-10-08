// Copyright of V.S. Puranam and no one else


#include "Core/KalkiPlayerController.h"

#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "KalkiGameplayTags.h"
#include "Characters/KalkiCharacter.h"

AKalkiPlayerController::AKalkiPlayerController()
{
}

AKalkiCharacter* AKalkiPlayerController::GetKalkiCharacter() const
{
	return Cast<AKalkiCharacter>(GetPawn());
}

#if WITH_EDITORONLY_DATA
void AKalkiPlayerController::DamageCharacter(float Amount)
{
	AKalkiCharacter* KalkiCharacter = GetKalkiCharacter();
	if (KalkiCharacter)
	{
		KalkiCharacter->ApplyHealthChange(-Amount);
		UE_LOG(LogTemp, Log, TEXT("Damaged character for %.0f"), Amount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Kalki character possessed"));
	}
}

void AKalkiPlayerController::HealCharacter(float Amount)
{
	AKalkiCharacter* KalkiCharacter = GetKalkiCharacter();
	if (KalkiCharacter)
	{
		KalkiCharacter->ApplyHealthChange(Amount);
		UE_LOG(LogTemp, Log, TEXT("Healed character for %.0f"), Amount);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Kalki character possessed"));
	}
}

void AKalkiPlayerController::ActivateMeleeAttack()
{
	using namespace KalkiGameplayTags;
	AKalkiCharacter* KalkiCharacter = GetKalkiCharacter();
	if (!KalkiCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("No character to activate ability"));
		return;
	}

	UAbilitySystemComponent* ASC = KalkiCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character has no ASC"));
		return;
	}

	// Try to activate ability by tag
	FGameplayTagContainer AbilityTags;
	AbilityTags.AddTag(Ability_Attack_Melee);
    
	bool bActivated = ASC->TryActivateAbilitiesByTag(AbilityTags);
    
	if (bActivated)
	{
		UE_LOG(LogTemp, Log, TEXT("Melee attack activated"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to activate melee attack"));
	}
}

void AKalkiPlayerController::ListAbilities()
{
	AKalkiCharacter* KalkiCharacter = GetKalkiCharacter();
	if (!KalkiCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("No character to list abilities"));
		return;
	}

	UAbilitySystemComponent* ASC = KalkiCharacter->GetAbilitySystemComponent();
	if (!ASC)
	{
		UE_LOG(LogTemp, Warning, TEXT("Character has no ASC"));
		return;
	}

	TArray<FGameplayAbilitySpec>& Specs = ASC->GetActivatableAbilities();
    
	UE_LOG(LogTemp, Log, TEXT("=== Abilities for %s ==="), *KalkiCharacter->GetName());
    
	if (Specs.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No abilities granted!"));
	}
	else
	{
		for (const FGameplayAbilitySpec& Spec : Specs)
		{
			if (Spec.Ability)
			{
				UE_LOG(LogTemp, Log, TEXT("- %s (Level %d)"), 
					*Spec.Ability->GetName(), Spec.Level);
			}
		}
	}
}
#endif
