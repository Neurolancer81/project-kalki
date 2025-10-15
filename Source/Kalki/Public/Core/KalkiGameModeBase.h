// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "KalkiGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class KALKI_API AKalkiGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AKalkiGameModeBase();

	// Override to allow cheat manager on clients (DEV ONLY)
	virtual bool AllowCheats(APlayerController* P) override;

protected:
	virtual void BeginPlay() override;

	
};
