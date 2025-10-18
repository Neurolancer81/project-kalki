// Copyright of V.S. Puranam and no one else

#include "Player/KalkiPlayerController.h"
#include "Player/KalkiCameraPawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Logging/KalkiLog.h"
#include "Player/Components/KalkiCharacterSelectionComponent.h"
#include "Player/Components/KalkiGridInteractionComponent.h"

AKalkiPlayerController::AKalkiPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableMouseOverEvents = true;

	// Create components
	GridInteractionComponent = CreateDefaultSubobject<UKalkiGridInteractionComponent>(TEXT("GridInteractionComponent"));
	CharacterSelectionComponent = CreateDefaultSubobject<UKalkiCharacterSelectionComponent>(TEXT("CharacterSelectionComponent"));
}

void AKalkiPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Spawn and possess camera pawn
	SpawnAndPossessCameraPawn();

	// Set up Enhanced Input
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (CameraInputMappingContext)
		{
			Subsystem->AddMappingContext(CameraInputMappingContext, InputMappingPriority);
			KalkiLog::System(TEXT("PlayerController - Camera input mapping context added"));
		}
		else
		{
			KalkiLog::System(
				TEXT("PlayerController - CameraInputMappingContext not set!"),
				EKalkiLogSeverity::Warning,
				this
			);
		}
	}

	KalkiLog::System(TEXT("KalkiPlayerController initialized"));
}

void AKalkiPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Get Enhanced Input Component
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Bind camera pan action (WASD, Arrows)
		if (PanCameraAction)
		{
			EnhancedInputComponent->BindAction(PanCameraAction, ETriggerEvent::Triggered, this, &AKalkiPlayerController::OnPanCamera);
			KalkiLog::System(TEXT("PlayerController - Pan Camera action bound"));
		}

		// Bind zoom action (Mouse Wheel)
		if (ZoomCameraAction)
		{
			EnhancedInputComponent->BindAction(ZoomCameraAction, ETriggerEvent::Triggered, this, &AKalkiPlayerController::OnZoomCamera);
			KalkiLog::System(TEXT("PlayerController - Zoom Camera action bound"));
		}

		// Bind rotate clockwise (E key)
		if (RotateClockwiseAction)
		{
			EnhancedInputComponent->BindAction(RotateClockwiseAction, ETriggerEvent::Started, this, &AKalkiPlayerController::OnRotateClockwise);
			KalkiLog::System(TEXT("PlayerController - Rotate Clockwise action bound"));
		}

		// Bind rotate counter-clockwise (Q key)
		if (RotateCounterClockwiseAction)
		{
			EnhancedInputComponent->BindAction(RotateCounterClockwiseAction, ETriggerEvent::Started, this, &AKalkiPlayerController::OnRotateCounterClockwise);
			KalkiLog::System(TEXT("PlayerController - Rotate Counter-Clockwise action bound"));
		}

		// Bind mouse clicks
		if (LeftClickAction)
		{
			EnhancedInputComponent->BindAction(LeftClickAction, ETriggerEvent::Started, this, &AKalkiPlayerController::OnLeftClick);
			KalkiLog::System(TEXT("PlayerController - Left Click action bound"));
		}
		else
		{
			KalkiLog::System(TEXT("PlayerController - LeftClickAction not set!"), EKalkiLogSeverity::Warning, this);
		}

		if (RightClickAction)
		{
			EnhancedInputComponent->BindAction(RightClickAction, ETriggerEvent::Started, this, &AKalkiPlayerController::OnRightClick);
			KalkiLog::System(TEXT("PlayerController - Right Click action bound"));
		}
		else
		{
			KalkiLog::System(TEXT("PlayerController - RightClickAction not set!"), EKalkiLogSeverity::Warning, this);
		}

		// Bind character selection (1-6 keys)
		if (SelectCharacter1Action)
		{
			EnhancedInputComponent->BindAction(SelectCharacter1Action, ETriggerEvent::Started, this, &AKalkiPlayerController::OnSelectCharacter1);
		}
		if (SelectCharacter2Action)
		{
			EnhancedInputComponent->BindAction(SelectCharacter2Action, ETriggerEvent::Started, this, &AKalkiPlayerController::OnSelectCharacter2);
		}
		if (SelectCharacter3Action)
		{
			EnhancedInputComponent->BindAction(SelectCharacter3Action, ETriggerEvent::Started, this, &AKalkiPlayerController::OnSelectCharacter3);
		}
		if (SelectCharacter4Action)
		{
			EnhancedInputComponent->BindAction(SelectCharacter4Action, ETriggerEvent::Started, this, &AKalkiPlayerController::OnSelectCharacter4);
		}
		// ✅ ADD THESE:
		if (SelectCharacter5Action)
		{
			EnhancedInputComponent->BindAction(SelectCharacter5Action, ETriggerEvent::Started, this, &AKalkiPlayerController::OnSelectCharacter5);
		}
		if (SelectCharacter6Action)
		{
			EnhancedInputComponent->BindAction(SelectCharacter6Action, ETriggerEvent::Started, this, &AKalkiPlayerController::OnSelectCharacter6);
		}

		// Bind cycle character (Tab)
		if (CycleCharacterAction)
		{
			EnhancedInputComponent->BindAction(CycleCharacterAction, ETriggerEvent::Started, this, &AKalkiPlayerController::OnCycleCharacter);
		}

		KalkiLog::System(TEXT("PlayerController - Character selection actions bound"));
	}
	else
	{
		KalkiLog::System(
			TEXT("PlayerController - Failed to get EnhancedInputComponent!"),
			EKalkiLogSeverity::Error,
			this
		);
	}
}

void AKalkiPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ========================================
// CAMERA PAWN
// ========================================

void AKalkiPlayerController::SpawnAndPossessCameraPawn()
{
	// Only on server/standalone
	if (!HasAuthority())
	{
		return;
	}

	// Check if we have a camera pawn class
	if (!CameraPawnClass)
	{
		KalkiLog::System(
			TEXT("PlayerController - No CameraPawnClass set! Cannot spawn camera."),
			EKalkiLogSeverity::Error,
			this
		);
		return;
	}

	// Spawn camera pawn
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	CameraPawn = GetWorld()->SpawnActor<AKalkiCameraPawn>(
		CameraPawnClass,
		CameraPawnSpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (CameraPawn)
	{
		// Possess the camera pawn
		Possess(CameraPawn);

		KalkiLog::System(
			FString::Printf(TEXT("PlayerController - Spawned and possessed CameraPawn at %s"), 
				*CameraPawnSpawnLocation.ToString())
		);
	}
	else
	{
		KalkiLog::System(
			TEXT("PlayerController - Failed to spawn CameraPawn!"),
			EKalkiLogSeverity::Error,
			this
		);
	}
}

// ========================================
// INPUT HANDLERS
// ========================================

void AKalkiPlayerController::OnPanCamera(const FInputActionValue& Value)
{
	if (!CameraPawn)
	{
		return;
	}

	// Get 2D input value (X = Forward/Backward, Y = Right/Left)
	const FVector2D PanValue = Value.Get<FVector2D>();

	// Forward/Backward (W/S or Up/Down arrows)
	if (FMath::Abs(PanValue.X) > SMALL_NUMBER)
	{
		CameraPawn->PanForward(PanValue.X);
	}

	// Right/Left (A/D or Left/Right arrows)
	if (FMath::Abs(PanValue.Y) > SMALL_NUMBER)
	{
		CameraPawn->PanRight(PanValue.Y);
	}
}

void AKalkiPlayerController::OnZoomCamera(const FInputActionValue& Value)
{
	if (!CameraPawn)
	{
		return;
	}

	// Get zoom value (mouse wheel axis)
	const float ZoomValue = Value.Get<float>();
	CameraPawn->Zoom(ZoomValue);
}

void AKalkiPlayerController::OnRotateClockwise(const FInputActionValue& Value)
{
	if (!CameraPawn)
	{
		return;
	}

	CameraPawn->RotateClockwise();
}

void AKalkiPlayerController::OnRotateCounterClockwise(const FInputActionValue& Value)
{
	if (!CameraPawn)
	{
		return;
	}

	CameraPawn->RotateCounterClockwise();
}

// ========================================
// MOUSE INPUT HANDLERS
// ========================================

void AKalkiPlayerController::OnLeftClick(const FInputActionValue& Value)
{
	if (GridInteractionComponent)
	{
		GridInteractionComponent->HandleLeftClick();
	}
	else
	{
		KalkiLog::System(
			TEXT("PlayerController - GridInteractionComponent not found!"),
			EKalkiLogSeverity::Warning,
			this
		);
	}
}

void AKalkiPlayerController::OnRightClick(const FInputActionValue& Value)
{
	if (GridInteractionComponent)
	{
		GridInteractionComponent->HandleRightClick();
	}
	else
	{
		KalkiLog::System(
			TEXT("PlayerController - GridInteractionComponent not found!"),
			EKalkiLogSeverity::Warning,
			this
		);
	}
}

// ========================================
// CHARACTER SELECTION INPUT HANDLERS
// ========================================

void AKalkiPlayerController::OnSelectCharacter1(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectCharacterBySlot(0);
	}
}

void AKalkiPlayerController::OnSelectCharacter2(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectCharacterBySlot(1);
	}
}

void AKalkiPlayerController::OnSelectCharacter3(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectCharacterBySlot(2);
	}
}

void AKalkiPlayerController::OnSelectCharacter4(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectCharacterBySlot(3);
	}
}

// ✅ ADD THESE:
void AKalkiPlayerController::OnSelectCharacter5(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectCharacterBySlot(4);
	}
}

void AKalkiPlayerController::OnSelectCharacter6(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectCharacterBySlot(5);
	}
}

void AKalkiPlayerController::OnCycleCharacter(const FInputActionValue& Value)
{
	if (CharacterSelectionComponent)
	{
		CharacterSelectionComponent->SelectNextCharacter();
	}
}