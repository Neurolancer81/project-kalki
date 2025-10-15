// Copyright of V.S. Puranam and no one else

#include "Core/KalkiPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Characters/KalkiCharacter.h"
#include "Debug/KalkiCheatManager.h"

AKalkiPlayerController::AKalkiPlayerController()
{
    bReplicates = true;
    
    // Set custom cheat manager
    CheatClass = UKalkiCheatManager::StaticClass();
}

AKalkiCharacter* AKalkiPlayerController::GetKalkiCharacter() const
{
    return Cast<AKalkiCharacter>(GetPawn());
}

void AKalkiPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Enable cheat manager on clients for development
#if !UE_BUILD_SHIPPING
    if (!CheatManager)
    {
        AddCheats(true); // Force enable cheat manager
    }
#endif

    if (!IsLocalPlayerController()) return;

    // Set up cursor and input modes
    bShowMouseCursor = true;
    DefaultMouseCursor = EMouseCursor::Default;

    FInputModeGameAndUI InputMode;
    InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
    InputMode.SetHideCursorDuringCapture(false);
    SetInputMode(InputMode);

    // Set up the input mapping context
    check(KalkiMappingContext);
    UEnhancedInputLocalPlayerSubsystem* SubSystem =
        ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
    check(SubSystem);

    SubSystem->AddMappingContext(KalkiMappingContext, 0);
}

void AKalkiPlayerController::Move(const FInputActionValue& ActionValue)
{
    const FVector2D InputAxisVector = ActionValue.Get<FVector2D>();
    const FRotator Rotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);

    const FVector ForwardDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
    const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

    if(APawn* ControlledPawn = GetPawn<APawn>())
    {
        ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
        ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
    }
}


void AKalkiPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();
    UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

    EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
    
    // Debug actions (NumPad keys)
    EnhancedInputComponent->BindAction(NumPad1Action, ETriggerEvent::Triggered, this, &ThisClass::NumPad1);
    EnhancedInputComponent->BindAction(NumPad2Action, ETriggerEvent::Triggered, this, &ThisClass::NumPad2);
    EnhancedInputComponent->BindAction(NumPad3Action, ETriggerEvent::Triggered, this, &ThisClass::NumPad3);
    EnhancedInputComponent->BindAction(NumPad4Action, ETriggerEvent::Triggered, this, &ThisClass::NumPad4);
}

void AKalkiPlayerController::NumPad1(const FInputActionValue& ActionValue)
{
    if (UKalkiCheatManager* Cheats = Cast<UKalkiCheatManager>(CheatManager))
    {
        Cheats->StartTestCombatLog();
    }
}

void AKalkiPlayerController::NumPad2(const FInputActionValue& ActionValue)
{
    if (UKalkiCheatManager* Cheats = Cast<UKalkiCheatManager>(CheatManager))
    {
        Cheats->ToggleUIMode();
    }
}

void AKalkiPlayerController::NumPad3(const FInputActionValue& ActionValue)
{
    if (UKalkiCheatManager* Cheats = Cast<UKalkiCheatManager>(CheatManager))
    {
        Cheats->ShowCombatUI();
    }
}

void AKalkiPlayerController::NumPad4(const FInputActionValue& ActionValue)
{
    if (UKalkiCheatManager* Cheats = Cast<UKalkiCheatManager>(CheatManager))
    {
        Cheats->ShowStrategyUI();
    }
}