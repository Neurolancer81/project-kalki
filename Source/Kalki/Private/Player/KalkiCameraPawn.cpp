// Copyright of V.S. Puranam and no one else

#include "Player/KalkiCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Logging/KalkiLog.h"

AKalkiCameraPawn::AKalkiCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create root scene component
	USceneComponent* RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;

	// Create spring arm
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = CameraDistance;
	CameraBoom->bDoCollisionTest = false; // No collision for strategic camera
	CameraBoom->bEnableCameraLag = true;  // Smooth camera movement (XCOM-style)
	CameraBoom->CameraLagSpeed = 12.0f;   // XCOM 2 has fairly responsive camera
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;

	// Create camera
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 75.0f; // XCOM 2 uses slightly wider FOV for tactical view

	// Create movement component
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->MaxSpeed = PanSpeed;
	MovementComponent->Acceleration = 4000.0f;  // XCOM 2 has snappy camera response
	MovementComponent->Deceleration = 8000.0f;  // Quick stop

	// Not possessed by default - PlayerController will possess this
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	// Initialize interpolation targets
	TargetCameraDistance = CameraDistance;
	TargetCameraYaw = CameraYaw;

	KalkiLog::System(TEXT("KalkiCameraPawn created"));
}

void AKalkiCameraPawn::BeginPlay()
{
	Super::BeginPlay();

	// Initialize camera settings
	UpdateCameraBoom();
	UpdateCameraRotation();

	KalkiLog::System(
		FString::Printf(TEXT("KalkiCameraPawn initialized at %s | Pitch: %.1f° | Yaw: %.1f° | Distance: %.0f"), 
			*GetActorLocation().ToString(),
			CameraPitch,
			CameraYaw,
			CameraDistance
		)
	);
}

void AKalkiCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smooth zoom interpolation
	if (bSmoothZoom)
	{
		InterpolateZoom(DeltaTime);
	}

	// Smooth rotation interpolation
	if (bSmoothRotation && bAllowRotation)
	{
		InterpolateRotation(DeltaTime);
	}

	// Apply camera bounds
	if (bUseCameraBounds)
	{
		ApplyCameraBounds();
	}

	// Handle edge scrolling
	if (bEnableEdgeScrolling)
	{
		HandleEdgeScrolling(DeltaTime);
	}
}

void AKalkiCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Note: Input will be handled by PlayerController using Enhanced Input
	// This function is here for potential future use
}

// ========================================
// CAMERA MOVEMENT
// ========================================

void AKalkiCameraPawn::PanForward(float Value)
{
	if (FMath::Abs(Value) < SMALL_NUMBER)
	{
		return;
	}

	// FIXED: Use world-space forward direction (Y-axis)
	// W/S always moves North/South regardless of camera rotation
	FVector WorldForward = FVector(0.0f, 1.0f, 0.0f);
	
	AddMovementInput(WorldForward, Value);
}

void AKalkiCameraPawn::PanRight(float Value)
{
	if (FMath::Abs(Value) < SMALL_NUMBER)
	{
		return;
	}

	// FIXED: Use world-space right direction (X-axis)
	// A/D always moves West/East regardless of camera rotation
	FVector WorldRight = FVector(1.0f, 0.0f, 0.0f);
	
	AddMovementInput(WorldRight, Value);
}

void AKalkiCameraPawn::Zoom(float Value)
{
	if (FMath::Abs(Value) < SMALL_NUMBER)
	{
		return;
	}

	// Adjust target camera distance
	TargetCameraDistance = FMath::Clamp(
		TargetCameraDistance - (Value * ZoomSpeed),
		MinZoomDistance,
		MaxZoomDistance
	);

	// If not using smooth zoom, update immediately
	if (!bSmoothZoom)
	{
		CameraDistance = TargetCameraDistance;
		UpdateCameraBoom();
	}

	KalkiLog::System(
		FString::Printf(TEXT("Camera zoom target: %.0f"), TargetCameraDistance),
		EKalkiLogSeverity::Verbose
	);
}

// ========================================
// CAMERA ROTATION
// ========================================

void AKalkiCameraPawn::RotateClockwise()
{
	if (!bAllowRotation)
	{
		return;
	}

	TargetCameraYaw = FMath::Fmod(TargetCameraYaw + RotationIncrement, 360.0f);

	// If not using smooth rotation, update immediately
	if (!bSmoothRotation)
	{
		CameraYaw = TargetCameraYaw;
		UpdateCameraRotation();
	}

	KalkiLog::System(
		FString::Printf(TEXT("Camera rotating clockwise to %.0f°"), TargetCameraYaw)
	);
}

void AKalkiCameraPawn::RotateCounterClockwise()
{
	if (!bAllowRotation)
	{
		return;
	}

	TargetCameraYaw = FMath::Fmod(TargetCameraYaw - RotationIncrement + 360.0f, 360.0f);

	// If not using smooth rotation, update immediately
	if (!bSmoothRotation)
	{
		CameraYaw = TargetCameraYaw;
		UpdateCameraRotation();
	}

	KalkiLog::System(
		FString::Printf(TEXT("Camera rotating counter-clockwise to %.0f°"), TargetCameraYaw)
	);
}

// ========================================
// CAMERA CONFIGURATION
// ========================================

void AKalkiCameraPawn::SetZoomDistance(float NewDistance)
{
	CameraDistance = FMath::Clamp(NewDistance, MinZoomDistance, MaxZoomDistance);
	TargetCameraDistance = CameraDistance;
	UpdateCameraBoom();
}

void AKalkiCameraPawn::SetCameraYaw(float NewYaw)
{
	CameraYaw = FMath::Fmod(NewYaw, 360.0f);
	TargetCameraYaw = CameraYaw;
	UpdateCameraRotation();
}

void AKalkiCameraPawn::SetCameraPitch(float NewPitch)
{
	CameraPitch = FMath::Clamp(NewPitch, -89.0f, -10.0f);
	UpdateCameraBoom();

	KalkiLog::System(
		FString::Printf(TEXT("Camera pitch set to %.1f°"), CameraPitch)
	);
}

void AKalkiCameraPawn::SnapToRotationIncrement()
{
	// Snap to nearest rotation increment
	float NormalizedYaw = FMath::Fmod(CameraYaw, 360.0f);
	float Remainder = FMath::Fmod(NormalizedYaw, RotationIncrement);
	
	if (Remainder < RotationIncrement * 0.5f)
	{
		CameraYaw = NormalizedYaw - Remainder;
	}
	else
	{
		CameraYaw = NormalizedYaw + (RotationIncrement - Remainder);
	}

	TargetCameraYaw = CameraYaw;
	UpdateCameraRotation();
}

FVector AKalkiCameraPawn::GetCameraWorldLocation() const
{
	if (Camera)
	{
		return Camera->GetComponentLocation();
	}
	return GetActorLocation();
}

FVector AKalkiCameraPawn::GetCameraForwardVector() const
{
	FRotator Rotation = FRotator(0.0f, CameraYaw, 0.0f);
	return Rotation.Vector();
}

FVector AKalkiCameraPawn::GetCameraRightVector() const
{
	FRotator Rotation = FRotator(0.0f, CameraYaw, 0.0f);
	return FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
}

// ========================================
// INTERNAL FUNCTIONS
// ========================================

void AKalkiCameraPawn::ApplyCameraBounds()
{
	FVector CurrentLocation = GetActorLocation();
	FVector ClampedLocation = CurrentLocation;

	ClampedLocation.X = FMath::Clamp(CurrentLocation.X, MinX, MaxX);
	ClampedLocation.Y = FMath::Clamp(CurrentLocation.Y, MinY, MaxY);

	if (!ClampedLocation.Equals(CurrentLocation, 0.1f))
	{
		SetActorLocation(ClampedLocation);
	}
}

void AKalkiCameraPawn::UpdateCameraBoom()
{
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = CameraDistance;

		// Set pitch for overhead view
		FRotator BoomRotation = FRotator(CameraPitch, 0.0f, 0.0f);
		CameraBoom->SetRelativeRotation(BoomRotation);
	}
}

void AKalkiCameraPawn::UpdateCameraRotation()
{
	if (CameraBoom)
	{
		// Rotate the spring arm (and thus the camera view) around the Z-axis
		// This rotates what the player sees, not the movement directions
		FRotator BoomRotation = FRotator(CameraPitch, CameraYaw, 0.0f);
		CameraBoom->SetWorldRotation(BoomRotation);
	}
	
	// DON'T rotate the pawn - we want world-space movement
	// Keep pawn at zero rotation so WASD stays in cardinal directions
}

void AKalkiCameraPawn::HandleEdgeScrolling(float DeltaTime)
{
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		return;
	}

	// Get viewport size
	int32 ViewportSizeX, ViewportSizeY;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Get mouse position
	float MouseX, MouseY;
	if (!PC->GetMousePosition(MouseX, MouseY))
	{
		return;
	}

	FVector2D PanDirection = FVector2D::ZeroVector;

	// Check edges
	if (MouseX < EdgeScrollingBorder)
	{
		PanDirection.Y = -1.0f; // Left
	}
	else if (MouseX > ViewportSizeX - EdgeScrollingBorder)
	{
		PanDirection.Y = 1.0f; // Right
	}

	if (MouseY < EdgeScrollingBorder)
	{
		PanDirection.X = 1.0f; // Forward (top of screen)
	}
	else if (MouseY > ViewportSizeY - EdgeScrollingBorder)
	{
		PanDirection.X = -1.0f; // Backward (bottom of screen)
	}

	// Apply edge scrolling movement
	if (!PanDirection.IsNearlyZero())
	{
		PanDirection.Normalize();
		PanForward(PanDirection.X * EdgeScrollingSpeed * DeltaTime);
		PanRight(PanDirection.Y * EdgeScrollingSpeed * DeltaTime);
	}
}

void AKalkiCameraPawn::InterpolateZoom(float DeltaTime)
{
	if (FMath::IsNearlyEqual(CameraDistance, TargetCameraDistance, 1.0f))
	{
		return;
	}

	CameraDistance = FMath::FInterpTo(
		CameraDistance,
		TargetCameraDistance,
		DeltaTime,
		ZoomInterpSpeed
	);

	UpdateCameraBoom();
}

void AKalkiCameraPawn::InterpolateRotation(float DeltaTime)
{
	// Calculate the shortest rotation path
	float DeltaYaw = TargetCameraYaw - CameraYaw;
	
	// Normalize to [-180, 180]
	while (DeltaYaw > 180.0f)
	{
		DeltaYaw -= 360.0f;
	}
	while (DeltaYaw < -180.0f)
	{
		DeltaYaw += 360.0f;
	}

	if (FMath::Abs(DeltaYaw) < 0.1f)
	{
		return;
	}

	// Interpolate
	CameraYaw += DeltaYaw * RotationInterpSpeed * DeltaTime;
	CameraYaw = FMath::Fmod(CameraYaw + 360.0f, 360.0f);

	UpdateCameraRotation();
}