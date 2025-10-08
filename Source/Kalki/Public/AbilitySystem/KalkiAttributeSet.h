// Public/Abilities/KalkiAttributeSet.h

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "KalkiAttributeSet.generated.h"

// Macros from AttributeSet.h for defining attributes
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class KALKI_API UKalkiAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UKalkiAttributeSet();

	// Vital attributes
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Attributes|Derived", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Attributes|Derived", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, MaxHealth)

	// Primary Attributes
	UPROPERTY(ReplicatedUsing=OnRep_Strength, BlueprintReadOnly, Category = "Kalki|Attributes|Primary")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Strength);

	UPROPERTY(ReplicatedUsing=OnRep_Dexterity, BlueprintReadOnly, Category = "Kalki|Attributes|Primary")
	FGameplayAttributeData Dexterity;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Dexterity);

	UPROPERTY(ReplicatedUsing=OnRep_Constitution, BlueprintReadOnly, Category = "Kalki|Attributes|Primary")
	FGameplayAttributeData Constitution;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Constitution);

	UPROPERTY(ReplicatedUsing=OnRep_Intelligence, BlueprintReadOnly, Category = "Kalki|Attributes|Primary")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Intelligence);

	UPROPERTY(ReplicatedUsing=OnRep_Wisdom, BlueprintReadOnly, Category = "Kalki|Attributes|Primary")
	FGameplayAttributeData Wisdom;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Wisdom);

	UPROPERTY(ReplicatedUsing=OnRep_Charisma, BlueprintReadOnly, Category = "Kalki|Attributes|Primary")
	FGameplayAttributeData Charisma;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, Charisma);

	// Derived stats
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Attributes|Derived", ReplicatedUsing = OnRep_ArmorClass)
	FGameplayAttributeData ArmorClass;
	ATTRIBUTE_ACCESSORS(UKalkiAttributeSet, ArmorClass)

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	// Rep notifies
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth) const;

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;	

	UFUNCTION()
	void OnRep_Dexterity(const FGameplayAttributeData& OldDexterity) const;

	UFUNCTION()
	void OnRep_Constitution(const FGameplayAttributeData& OldConstitution) const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;

	UFUNCTION()
	void OnRep_Wisdom(const FGameplayAttributeData& OldWisdom) const;

	UFUNCTION()
	void OnRep_Charisma(const FGameplayAttributeData& OldCharisma) const;

	UFUNCTION()
	void OnRep_ArmorClass(const FGameplayAttributeData& OldArmorClass) const;

	// Called just before an attribute's value is changed (allows clamping)
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;

	// Called after an attribute changes (for side effects like death)
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
};