```cpp
// Copyright of V.S. Puranam and no one else  
  
#pragma once  
  
#include "CoreMinimal.h"  
#include "Components/ActorComponent.h"  
#include "Grid/KalkiGridTypes.h"  
#include "KalkiGridInteractionComponent.generated.h"  
  
// Forward declarations  
class UKalkiGridManager;  
class AKalkiGridVisualizer;  
  
/**  
 * Grid Interaction Component * Handles mouse interaction with the grid (clicking, raycasting, selection) * Attached to PlayerController */UCLASS(ClassGroup=(Kalki), meta=(BlueprintSpawnableComponent))  
class KALKI_API UKalkiGridInteractionComponent : public UActorComponent  
{  
    GENERATED_BODY()  
  
public:  
    UKalkiGridInteractionComponent();  
  
protected:  
    virtual void BeginPlay() override;  
  
public:  
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;  
  
    // ========================================  
    // SETTINGS    // ========================================  
    /** Enable click detection? */    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")  
    bool bEnableClickDetection = true;  
  
    /** Show debug visualizations? */  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")  
    bool bShowDebug = false;  
  
    /** Raycast channel for grid detection */  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")  
    TEnumAsByte<ECollisionChannel> RaycastChannel = ECC_Visibility;  
  
    /** Maximum raycast distance */  
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kalki|Grid Interaction")  
    float MaxRaycastDistance = 50000.0f;  
  
    // ========================================  
    // STATE    // ========================================  
    /** Currently selected tile */    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Interaction")  
    FKalkiGridCoord SelectedTile;  
  
    /** Is a tile selected? */  
    UPROPERTY(BlueprintReadOnly, Category = "Kalki|Grid Interaction")  
    bool bHasSelection = false;  
  
    // ========================================  
    // PUBLIC FUNCTIONS    // ========================================  
    /** Handle left mouse click */    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")  
    void HandleLeftClick();  
  
    /** Handle right mouse click */  
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")  
    void HandleRightClick();  
  
    /** Raycast from camera to world, return hit tile */  
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")  
    bool RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord);  
  
    /** Get tile under mouse cursor */  
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")  
    FKalkiGridCoord GetTileUnderCursor(bool& bSuccess);  
  
    /** Select a tile */  
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")  
    void SelectTile(const FKalkiGridCoord& Coord);  
  
    /** Clear tile selection */  
    UFUNCTION(BlueprintCallable, Category = "Kalki|Grid Interaction")  
    void ClearSelection();  
  
    /** Get selected tile (returns invalid coord if none selected) */  
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Interaction")  
    FKalkiGridCoord GetSelectedTile() const { return SelectedTile; }  
  
    /** Is a tile currently selected? */  
    UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Kalki|Grid Interaction")  
    bool HasSelection() const { return bHasSelection; }  
  
private:  
    // ========================================  
    // CACHED REFERENCES    // ========================================  
    UPROPERTY()  
    TObjectPtr<UKalkiGridManager> GridManager;  
  
    UPROPERTY()  
    TObjectPtr<AKalkiGridVisualizer> GridVisualizer;  
  
    UPROPERTY()  
    TObjectPtr<APlayerController> OwningController;  
  
    // ========================================  
    // INTERNAL FUNCTIONS    // ========================================  
    /** Cache subsystem references */    void CacheReferences();  
  
    /** Draw debug visuals */  
    void DrawDebugVisuals(const FVector& Start, const FVector& End, bool bHit, const FVector& HitLocation);  
};
```

```cpp
// Copyright of V.S. Puranam and no one else  
  
#include "Player/Components/KalkiGridInteractionComponent.h"  
#include "Grid/KalkiGridManager.h"  
#include "Grid/KalkiGridVisualizer.h"  
#include "GameFramework/PlayerController.h"  
#include "Engine/World.h"  
#include "DrawDebugHelpers.h"  
#include "Kismet/GameplayStatics.h"  
#include "Logging/KalkiLog.h"  
  
UKalkiGridInteractionComponent::UKalkiGridInteractionComponent()  
{  
    PrimaryComponentTick.bCanEverTick = true;  
    PrimaryComponentTick.bStartWithTickEnabled = true;  
}  
  
void UKalkiGridInteractionComponent::BeginPlay()  
{  
    Super::BeginPlay();  
  
    CacheReferences();  
  
    KalkiLog::System(TEXT("GridInteractionComponent initialized"));  
}  
  
void UKalkiGridInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)  
{  
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);  
  
    // Draw debug visuals if enabled  
    if (bShowDebug && bEnableClickDetection)  
    {       FVector HitLocation;  
       FKalkiGridCoord Coord;  
       FVector Start, End;  
       if (OwningController)  
       {          OwningController->DeprojectMousePositionToWorld(Start, End);  
          End = Start + (End * MaxRaycastDistance);  
          bool bHit = RaycastToGrid(HitLocation, Coord);  
          DrawDebugVisuals(Start, End, bHit, HitLocation);  
       }    }}  
  
// ========================================  
// PUBLIC FUNCTIONS  
// ========================================  
  
void UKalkiGridInteractionComponent::HandleLeftClick()  
{  
    if (!bEnableClickDetection)  
    {       return;  
    }  
    FVector HitLocation;  
    FKalkiGridCoord ClickedCoord;  
  
    if (RaycastToGrid(HitLocation, ClickedCoord))  
    {       // Valid tile clicked  
       SelectTile(ClickedCoord);  
  
       KalkiLog::Grid(  
          FString::Printf(TEXT("Tile clicked: %s"), *ClickedCoord.ToString())  
       );    }    else  
    {  
       // Clicked outside grid  
       ClearSelection();  
  
       KalkiLog::Grid(  
          TEXT("Click outside grid - selection cleared"),  
          EKalkiLogSeverity::Verbose  
       );  
    }}  
  
void UKalkiGridInteractionComponent::HandleRightClick()  
{  
    if (!bEnableClickDetection)  
    {       return;  
    }  
    // Right click clears selection (or could be context menu in future)  
    ClearSelection();  
  
    KalkiLog::Grid(TEXT("Right click - selection cleared"), EKalkiLogSeverity::Verbose);  
}  
  
bool UKalkiGridInteractionComponent::RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord)  
{  
    if (!OwningController || !GridManager)  
    {       return false;  
    }  
    // Get mouse ray  
    FVector WorldLocation, WorldDirection;  
    if (!OwningController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))  
    {       return false;  
    }  
    // Perform raycast  
    FHitResult HitResult;  
    FVector TraceEnd = WorldLocation + (WorldDirection * MaxRaycastDistance);  
    FCollisionQueryParams QueryParams;  
    QueryParams.bTraceComplex = false;  
  
    bool bHit = GetWorld()->LineTraceSingleByChannel(  
       HitResult,       WorldLocation,       TraceEnd,       RaycastChannel,  
       QueryParams    );  
    if (bHit)  
    {       OutHitLocation = HitResult.Location;  
  
       // Convert world position to grid coordinate  
       OutCoord = GridManager->WorldPositionToCoord(OutHitLocation);  
  
       // Validate coordinate  
       if (GridManager->IsValidCoord(OutCoord))  
       {          return true;  
       }    }  
    return false;  
}  
  
FKalkiGridCoord UKalkiGridInteractionComponent::GetTileUnderCursor(bool& bSuccess)  
{  
    FVector HitLocation;  
    FKalkiGridCoord Coord;  
  
    bSuccess = RaycastToGrid(HitLocation, Coord);  
    return Coord;  
}  
  
void UKalkiGridInteractionComponent::SelectTile(const FKalkiGridCoord& Coord)  
{  
    if (!GridManager || !GridManager->IsValidCoord(Coord))  
    {       KalkiLog::Grid(  
          FString::Printf(TEXT("Cannot select invalid tile: %s"), *Coord.ToString()),  
          EKalkiLogSeverity::Warning  
       );  
       return;  
    }  
    // Update selection state  
    SelectedTile = Coord;  
    bHasSelection = true;  
  
    // Update visualizer  
    if (GridVisualizer)  
    {       GridVisualizer->SelectTile(Coord);  
    }  
    KalkiLog::Grid(  
       FString::Printf(TEXT("Tile selected: %s"), *Coord.ToString())  
    );}  
  
void UKalkiGridInteractionComponent::ClearSelection()  
{  
    if (!bHasSelection)  
    {       return;  
    }  
    bHasSelection = false;  
    SelectedTile = FKalkiGridCoord::Invalid();  
  
    // Clear visualizer  
    if (GridVisualizer)  
    {       GridVisualizer->DeselectTile();  
    }  
    KalkiLog::Grid(TEXT("Tile selection cleared"), EKalkiLogSeverity::Verbose);  
}  
  
// ========================================  
// INTERNAL FUNCTIONS  
// ========================================  
  
void UKalkiGridInteractionComponent::CacheReferences()  
{  
    // Get owning controller  
    OwningController = Cast<APlayerController>(GetOwner());  
    if (!OwningController)  
    {       KalkiLog::System(  
          TEXT("GridInteractionComponent - Owner is not a PlayerController!"),  
          EKalkiLogSeverity::Error,  
          this  
       );  
       return;  
    }  
    // Get grid manager  
    GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();  
    if (!GridManager)  
    {       KalkiLog::System(  
          TEXT("GridInteractionComponent - GridManager not found!"),  
          EKalkiLogSeverity::Warning,  
          this  
       );  
    }  
    // Find grid visualizer  
    // Note: This might not exist at BeginPlay, so we'll try again if needed    TArray<AActor*> FoundActors;  
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiGridVisualizer::StaticClass(), FoundActors);  
    if (FoundActors.Num() > 0)  
    {       GridVisualizer = Cast<AKalkiGridVisualizer>(FoundActors[0]);  
       if (GridVisualizer)  
       {          KalkiLog::System(TEXT("GridInteractionComponent - Found GridVisualizer"));  
       }    }}  
  
void UKalkiGridInteractionComponent::DrawDebugVisuals(const FVector& Start, const FVector& End, bool bHit, const FVector& HitLocation)  
{  
    if (!GetWorld())  
    {       return;  
    }  
    // Draw raycast line  
    FColor LineColor = bHit ? FColor::Green : FColor::Red;  
    DrawDebugLine(  
       GetWorld(),  
       Start,       bHit ? HitLocation : End,  
       LineColor,       false,  
       0.0f,  
       0,  
       2.0f  
    );  
  
    // Draw hit point  
    if (bHit)  
    {       DrawDebugSphere(  
          GetWorld(),  
          HitLocation,          25.0f,  
          12,  
          FColor::Yellow,  
          false,  
          0.0f  
       );  
    }}
```