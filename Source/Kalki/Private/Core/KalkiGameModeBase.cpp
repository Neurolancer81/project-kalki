// Copyright of V.S. Puranam and no one else


#include "Core/KalkiGameModeBase.h"
#include "Characters/KalkiCharacter.h"
#include "Core/KalkiPlayerController.h"
#include "Core/KalkiPlayerState.h"

AKalkiGameModeBase::AKalkiGameModeBase()
{
	// Set default classes
	PlayerControllerClass = AKalkiPlayerController::StaticClass();
	PlayerStateClass = AKalkiPlayerState::StaticClass();
	DefaultPawnClass = AKalkiCharacter::StaticClass();
}
