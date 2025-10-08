// Private/Abilities/KalkiAttributeSet.cpp

#include "AbilitySystem//KalkiAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Characters/KalkiCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Net/UnrealNetwork.h"

UKalkiAttributeSet::UKalkiAttributeSet()
{
}

void UKalkiAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	// Primary Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Strength, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Dexterity, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Constitution, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Intelligence, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Wisdom, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Charisma, COND_None, REPNOTIFY_Always);

	// Derived Attributes
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, ArmorClass, COND_None, REPNOTIFY_Always);
}

void UKalkiAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKalkiAttributeSet, Health, OldHealth);
}

void UKalkiAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UKalkiAttributeSet, MaxHealth, OldMaxHealth);
}

void UKalkiAttributeSet::OnRep_Strength(const FGameplayAttributeData& OldStrength) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Strength, OldStrength);
}

void UKalkiAttributeSet::OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Dexterity, OldDexterity);
}

void UKalkiAttributeSet::OnRep_Constitution(const FGameplayAttributeData& OldConstitution) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Constitution, OldConstitution);
}

void UKalkiAttributeSet::OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Intelligence, OldIntelligence);
}

void UKalkiAttributeSet::OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Wisdom, OldWisdom);
}

void UKalkiAttributeSet::OnRep_Charisma(const FGameplayAttributeData& OldCharisma) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Charisma, OldCharisma);
}

void UKalkiAttributeSet::OnRep_ArmorClass(const FGameplayAttributeData& OldArmorClass) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, ArmorClass, OldArmorClass);
}

void UKalkiAttributeSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	// Clamp Health to valid range [0, MaxHealth]
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
    
	// Clamp MaxHealth to minimum of 1
	if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max(1.0f, NewValue);
	}
}

void UKalkiAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		// Clamp health
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		// Check for death
		if (GetHealth() <= 0.0f)
		{
			// Get the owning actor from the ASC, not from AttributeSet directly
			if (UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent())
			{
				if (AActor* AvatarActor = ASC->GetAvatarActor())
				{
					UE_LOG(LogTemp, Warning, TEXT("%s has died!"), *AvatarActor->GetName());

					// Try to cast to KalkiCharacter
					if (const AKalkiCharacter* KalkiCharacter = Cast<AKalkiCharacter>(AvatarActor))
					{
						// Disable collision
						if (UCapsuleComponent* Capsule = KalkiCharacter->GetCapsuleComponent())
						{
							Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
						}

						// Optional: Ragdoll
						if (USkeletalMeshComponent* Mesh = KalkiCharacter->GetMesh())
						{
							Mesh->SetSimulatePhysics(true);
							Mesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
						}
					}
				}
			}
		}
	}
}
