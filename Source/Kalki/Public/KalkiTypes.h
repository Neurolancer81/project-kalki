// Public/KalkiTypes.h

#pragma once

#include "CoreMinimal.h"
#include "KalkiTypes.generated.h"

class UGameplayAbility;
class UKalkiGameplayAbility;

// Character class enum
UENUM(BlueprintType)
enum class EKalkiCharacterClass : uint8
{
	None        UMETA(DisplayName = "None"),
	Fighter     UMETA(DisplayName = "Fighter"),
	Mage        UMETA(DisplayName = "Mage"),
	Cleric      UMETA(DisplayName = "Cleric"),
	Rogue       UMETA(DisplayName = "Rogue"),
	// Add more as needed
};

// Damage types
UENUM(BlueprintType)
enum class EKalkiDamageType : uint8
{
	Physical    UMETA(DisplayName = "Physical"),
	Fire        UMETA(DisplayName = "Fire"),
	Cold        UMETA(DisplayName = "Cold"),
	Lightning   UMETA(DisplayName = "Lightning"),
	Poison      UMETA(DisplayName = "Poison"),
	// etc.
};

// Attribute definition (used by rulesets)
USTRUCT(BlueprintType)
struct FKalkiAttributeDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AttributeName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BaseValue = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinValue = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxValue = 30.0f;
};

// Character data (creation and persistence)
USTRUCT(BlueprintType)
struct FKalkiCharacterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EKalkiCharacterClass Class = EKalkiCharacterClass::Fighter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, float> BaseAttributes; // "Strength": 14, "Dexterity": 12, etc.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
};

UENUM(BlueprintType)
enum class EKalkiTerrainType : uint8
{
	Normal      UMETA(DisplayName = "Normal"),
	Difficult   UMETA(DisplayName = "Difficult Terrain"),
	Wall        UMETA(DisplayName = "Wall"),
	Water       UMETA(DisplayName = "Water"),
	Hazard      UMETA(DisplayName = "Hazard"),
	Cover       UMETA(DisplayName = "Cover")
};