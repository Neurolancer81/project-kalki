// Copyright of V.S. Puranam and no one else

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "KalkiPlayerController.generated.h"

class UKalkiCharacterSelectionComponent;
class UKalkiGridInteractionComponent;
// Forward declarations
class AKalkiCameraPawn;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * Kalki Player Controller
 * Handles player input and camera control for tactical gameplay
 */
UCLASS()
class KALKI_API AKalkiPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AKalkiPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

public:
	virtual void Tick(float DeltaTime) override;

	// ========================================
	// ENHANCED INPUT
	// ========================================

	/** Input Mapping Context for camera controls */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input")
	TObjectPtr<UInputMappingContext> CameraInputMappingContext;

	/** Input Action: Pan Camera (WASD, Arrow Keys) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Camera")
	TObjectPtr<UInputAction> PanCameraAction;

	/** Input Action: Zoom Camera (Mouse Wheel) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Camera")
	TObjectPtr<UInputAction> ZoomCameraAction;

	/** Input Action: Rotate Camera Clockwise (E key) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Camera")
	TObjectPtr<UInputAction> RotateClockwiseAction;

	/** Input Action: Rotate Camera Counter-Clockwise (Q key) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Camera")
	TObjectPtr<UInputAction> RotateCounterClockwiseAction;

	/** Input Mapping Context priority */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input")
	int32 InputMappingPriority = 0;

	/** Input Action: Left Mouse Click */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Mouse")
	TObjectPtr<UInputAction> LeftClickAction;

	/** Input Action: Right Mouse Click */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Mouse")
	TObjectPtr<UInputAction> RightClickAction;

	// ========================================
	// CAMERA PAWN
	// ========================================

	/** Reference to the camera pawn we're controlling */
	UPROPERTY(BlueprintReadOnly, Category = "Kalki|Camera")
	TObjectPtr<AKalkiCameraPawn> CameraPawn;

	/** Camera pawn class to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera")
	TSubclassOf<AKalkiCameraPawn> CameraPawnClass;

	/** Camera pawn spawn location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Camera")
	FVector CameraPawnSpawnLocation = FVector(0.0f, 0.0f, 0.0f);

	// ========================================
	// FUNCTIONS
	// ========================================

	/** Spawn and possess camera pawn */
	UFUNCTION(BlueprintCallable, Category = "Kalki|Camera")
	void SpawnAndPossessCameraPawn();

	/** Get the camera pawn we're controlling */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Camera")
	AKalkiCameraPawn* GetCameraPawn() const { return CameraPawn; }

	// ========================================
	// GRID INTERACTION
	// ========================================

	/** Grid interaction component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Grid")
	TObjectPtr<UKalkiGridInteractionComponent> GridInteractionComponent;

	// ========================================
	// CHARACTER SELECTION
	// ========================================

	/** Character selection component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kalki|Character")
	TObjectPtr<UKalkiCharacterSelectionComponent> CharacterSelectionComponent;

	// Add with other character selection actions:

	/** Input Actions for character selection (1-6 keys) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> SelectCharacter1Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> SelectCharacter2Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> SelectCharacter3Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> SelectCharacter4Action;

	// ✅ ADD THESE TWO:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> SelectCharacter5Action;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> SelectCharacter6Action;

	/** Input Action: Cycle character selection (Tab) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kalki|Input|Character")
	TObjectPtr<UInputAction> CycleCharacterAction;

protected:
	// ========================================
	// INPUT HANDLERS
	// ========================================

	/** Handle camera panning (WASD, Arrows) */
	void OnPanCamera(const FInputActionValue& Value);

	/** Handle camera zoom (Mouse Wheel) */
	void OnZoomCamera(const FInputActionValue& Value);

	/** Handle camera rotation clockwise (E key) */
	void OnRotateClockwise(const FInputActionValue& Value);

	/** Handle camera rotation counter-clockwise (Q key) */
	void OnRotateCounterClockwise(const FInputActionValue& Value);

	// ========================================
	// GRID INTERACTION
	// ========================================
	
	/** Handle left mouse click */
	void OnLeftClick(const FInputActionValue& Value);

	/** Handle right mouse click */
	void OnRightClick(const FInputActionValue& Value);

	// ========================================
	// CHARACTER SELECTION
	// ========================================
	
	/** Handle character selection by number */
	void OnSelectCharacter1(const FInputActionValue& Value);
	void OnSelectCharacter2(const FInputActionValue& Value);
	void OnSelectCharacter3(const FInputActionValue& Value);
	void OnSelectCharacter4(const FInputActionValue& Value);
	void OnSelectCharacter5(const FInputActionValue& Value); 
	void OnSelectCharacter6(const FInputActionValue& Value); 

	/** Handle cycle character (Tab) */
	void OnCycleCharacter(const FInputActionValue& Value);
};