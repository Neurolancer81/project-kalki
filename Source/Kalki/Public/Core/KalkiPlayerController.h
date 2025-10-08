// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KalkiPlayerController.generated.h"

class AKalkiCharacter;
/**
 * 
 */
UCLASS()
class KALKI_API AKalkiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKalkiPlayerController();

#if WITH_EDITORONLY_DATA

	UFUNCTION(Exec, Category = "Kalki|Debug")
	void DamageCharacter(float Amount);

	UFUNCTION(Exec, Category = "Kalki|Debug")
	void HealCharacter(float Amount);

	UFUNCTION(Exec, Category = "Kalki|Debug")
	void ActivateMeleeAttack();

	UFUNCTION(Exec, Category = "Kalki|Debug")
	void ListAbilities();

#endif

protected:
	// Helper to get the controlled Kalki character
	AKalkiCharacter* GetKalkiCharacter() const;
};
