// Public/Characters/KalkiCharacter.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "KalkiTypes.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "KalkiCharacter.generated.h"

class UKalkiDnD5eRuleset;
class UKalkiAttributeSet;
class UKalkiAbilitySystemComponent;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS()
class KALKI_API AKalkiCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AKalkiCharacter(const FObjectInitializer& ObjectInitializer);

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Initialize the ability system (called when possessed or player state is set)
	virtual void InitializeAbilitySystem();

	// Getter for AS
	UKalkiAttributeSet* GetAttributeSet() const {return AttributeSet;};

	/* START DEBUG STUFF */

	/* END DEBUG STUFF */

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Camera components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera")
	float CameraDistance = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera")
	float CameraPitch = -50.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UKalkiAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UKalkiAttributeSet> AttributeSet;

	// Track if we've initialized GAS
	bool bAbilitySystemInitialized;

	// Character definition data
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Character", meta = (AllowPrivateAccess = "true"))
	FKalkiCharacterData CharacterData;

	// The active ruleset for this character
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kalki|Ruleset")
	TSubclassOf<UKalkiDnD5eRuleset> RulesetClass;

	UPROPERTY()
	TObjectPtr<UKalkiDnD5eRuleset> ActiveRuleset;

/********************************* START DEBUG STUFF ************************************************/
public:
	// Debug visualization
	UFUNCTION(BlueprintCallable, Category = "Kalki|Debug")
	void DrawDebugStats() const;

	// Or simpler - apply damage/heal directly through GAS
	void ApplyHealthChange(float Delta);

protected:
	// Tick to draw debug info
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Debug")
	bool bShowDebugStats = true;



/***************************** END DEBUG STUFF ************************************************/
};