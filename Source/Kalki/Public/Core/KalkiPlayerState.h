// Public/Core/KalkiPlayerState.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "KalkiPlayerState.generated.h"

class AKalkiCharacter;

UCLASS()
class KALKI_API AKalkiPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AKalkiPlayerState(const FObjectInitializer& ObjectInitializer);

	// Team management
	UFUNCTION(BlueprintCallable, Category = "Kalki|Team")
	const TArray<AKalkiCharacter*>& GetControlledCharacters() const { return ControlledCharacters; }

	UFUNCTION(BlueprintCallable, Category = "Kalki|Team")
	void AddControlledCharacter(AKalkiCharacter* Character);

	UFUNCTION(BlueprintCallable, Category = "Kalki|Team")
	void RemoveControlledCharacter(AKalkiCharacter* Character);

protected:
	// Characters this player controls
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Team")
	TArray<TObjectPtr<AKalkiCharacter>> ControlledCharacters;

	// Player-level resources (optional, for future)
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Kalki|Resources")
	int32 TeamResources;

	// Replication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};