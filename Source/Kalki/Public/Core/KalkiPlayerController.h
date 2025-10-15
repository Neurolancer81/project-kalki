// Copyright of V.S. Puranam and no one else

// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KalkiPlayerController.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class AKalkiCharacter;

UCLASS()
class KALKI_API AKalkiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKalkiPlayerController();

protected:
	// Helper to get the controlled Kalki character
	AKalkiCharacter* GetKalkiCharacter() const;

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* KalkiMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input|Debug")
	TObjectPtr<UInputAction> NumPad1Action;  // StartTestCombatLog

	UPROPERTY(EditAnywhere, Category = "Input|Debug")
	TObjectPtr<UInputAction> NumPad2Action;  // ToggleUIMode

	UPROPERTY(EditAnywhere, Category = "Input|Debug")
	TObjectPtr<UInputAction> NumPad3Action;  // ShowCombatUI

	UPROPERTY(EditAnywhere, Category = "Input|Debug")
	TObjectPtr<UInputAction> NumPad4Action;  // ShowStrategyUI

	void Move(const FInputActionValue& ActionValue);
	void NumPad1(const FInputActionValue& ActionValue);
	void NumPad2(const FInputActionValue& ActionValue);
	void NumPad3(const FInputActionValue& ActionValue);
	void NumPad4(const FInputActionValue& ActionValue);
};