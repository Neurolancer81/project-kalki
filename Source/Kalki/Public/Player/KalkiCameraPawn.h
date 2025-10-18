// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "KalkiCameraPawn.generated.h"

// Forward declarations
class USpringArmComponent;
class UCameraComponent;
class UFloatingPawnMovement;

/**
 * Kalki Camera Pawn
 * Strategic overhead camera for tactical gameplay
 * Handles WASD panning, zoom, and rotation
 * NOT a character - just a camera controller
 * 
 * Default settings inspired by XCOM 2's tactical camera
 */
UCLASS()
class KALKI_API AKalkiCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	AKalkiCameraPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ========================================
	// COMPONENTS
	// ========================================

	/** Spring arm for camera positioning */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Camera component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Camera")
	TObjectPtr<UCameraComponent> Camera;

	/** Movement component for smooth panning */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Camera")
	TObjectPtr<UFloatingPawnMovement> MovementComponent;

	// ========================================
	// CAMERA SETTINGS - Distance & Zoom
	// ========================================

	/** Camera distance from ground (adjustable via zoom) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Distance")
	float CameraDistance = 1800.0f;

	/** Minimum zoom distance (closest to ground) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Distance")
	float MinZoomDistance = 1000.0f;

	/** Maximum zoom distance (farthest from ground) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Distance")
	float MaxZoomDistance = 3500.0f;

	/** Zoom speed (units per mouse wheel step) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Distance")
	float ZoomSpeed = 150.0f;

	/** Enable smooth zoom interpolation? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Distance")
	bool bSmoothZoom = true;

	/** Smooth zoom interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Distance", meta = (EditCondition = "bSmoothZoom"))
	float ZoomInterpSpeed = 10.0f;

	// ========================================
	// CAMERA SETTINGS - Angle & Rotation
	// ========================================

	/** Camera pitch angle in degrees (negative = looking down)
	 *  -45° = XCOM 2 style (default)
	 *  -60° = Steeper overhead
	 *  -30° = More horizontal
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Angle", meta = (ClampMin = "-89.0", ClampMax = "-10.0"))
	float CameraPitch = -45.0f;

	/** Current camera rotation (yaw) in degrees */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Angle")
	float CameraYaw = 45.0f;  // Start at 45° for nice diagonal view

	/** Allow camera rotation? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Angle")
	bool bAllowRotation = true;

	/** Rotation angle increment in degrees (Q/E keys)
	 *  45° = Smooth 8-direction rotation (default)
	 *  90° = XCOM-style 4-direction rotation
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Angle", meta = (ClampMin = "1.0", ClampMax = "180.0"))
	float RotationIncrement = 45.0f;

	/** Enable smooth rotation interpolation? */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Angle")
	bool bSmoothRotation = true;

	/** Smooth rotation interpolation speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Angle", meta = (EditCondition = "bSmoothRotation"))
	float RotationInterpSpeed = 8.0f;

	// ========================================
	// CAMERA SETTINGS - Movement
	// ========================================

	/** Pan speed multiplier (WASD/Arrow keys) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Movement")
	float PanSpeed = 1200.0f;

	/** Edge scrolling enabled? (mouse at screen edge pans camera) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Movement")
	bool bEnableEdgeScrolling = false;

	/** Edge scrolling activation distance from screen edge (pixels) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Movement", meta = (EditCondition = "bEnableEdgeScrolling"))
	float EdgeScrollingBorder = 20.0f;

	/** Edge scrolling speed multiplier */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Movement", meta = (EditCondition = "bEnableEdgeScrolling"))
	float EdgeScrollingSpeed = 800.0f;

	// ========================================
	// CAMERA BOUNDS (Optional)
	// ========================================

	/** Enable camera bounds? (restrict camera to map area) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Bounds")
	bool bUseCameraBounds = true;

	/** Minimum X position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Bounds", meta = (EditCondition = "bUseCameraBounds"))
	float MinX = -5000.0f;

	/** Maximum X position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Bounds", meta = (EditCondition = "bUseCameraBounds"))
	float MaxX = 5000.0f;

	/** Minimum Y position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Bounds", meta = (EditCondition = "bUseCameraBounds"))
	float MinY = -5000.0f;

	/** Maximum Y position */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera|Bounds", meta = (EditCondition = "bUseCameraBounds"))
	float MaxY = 5000.0f;

	// ========================================
	// PUBLIC FUNCTIONS
	// ========================================

	/** Pan camera forward/backward (WASD W/S, Arrow Up/Down) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void PanForward(float Value);

	/** Pan camera right/left (WASD A/D, Arrow Left/Right) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void PanRight(float Value);

	/** Zoom camera in/out (Mouse wheel) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void Zoom(float Value);

	/** Rotate camera clockwise by increment (E key) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void RotateClockwise();

	/** Rotate camera counter-clockwise by increment (Q key) */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void RotateCounterClockwise();

	/** Set camera zoom distance directly */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void SetZoomDistance(float NewDistance);

	/** Set camera yaw rotation directly */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void SetCameraYaw(float NewYaw);

	/** Set camera pitch angle directly */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void SetCameraPitch(float NewPitch);

	/** Get current camera world location */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Camera")
	FVector GetCameraWorldLocation() const;

	/** Get camera forward vector (for panning) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Camera")
	FVector GetCameraForwardVector() const;

	/** Get camera right vector (for panning) */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Camera")
	FVector GetCameraRightVector() const;

	/** Snap camera to nearest rotation increment */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void SnapToRotationIncrement();

private:
	// ========================================
	// INTERNAL STATE
	// ========================================

	/** Target zoom distance (for smooth interpolation) */
	float TargetCameraDistance;

	/** Target yaw rotation (for smooth interpolation) */
	float TargetCameraYaw;

	// ========================================
	// INTERNAL FUNCTIONS
	// ========================================

	/** Apply camera bounds to current position */
	void ApplyCameraBounds();

	/** Update camera boom settings */
	void UpdateCameraBoom();

	/** Update camera rotation */
	void UpdateCameraRotation();

	/** Handle edge scrolling */
	void HandleEdgeScrolling(float DeltaTime);

	/** Interpolate zoom smoothly */
	void InterpolateZoom(float DeltaTime);

	/** Interpolate rotation smoothly */
	void InterpolateRotation(float DeltaTime);
};