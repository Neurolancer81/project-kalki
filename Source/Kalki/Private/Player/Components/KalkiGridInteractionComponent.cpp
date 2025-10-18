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
	{
		FVector HitLocation;
		FKalkiGridCoord Coord;
		
		FVector Start, End;
		if (OwningController)
		{
			OwningController->DeprojectMousePositionToWorld(Start, End);
			End = Start + (End * MaxRaycastDistance);
			
			bool bHit = RaycastToGrid(HitLocation, Coord);
			DrawDebugVisuals(Start, End, bHit, HitLocation);
		}
	}
}

// ========================================
// PUBLIC FUNCTIONS
// ========================================

void UKalkiGridInteractionComponent::HandleLeftClick()
{
	if (!bEnableClickDetection)
	{
		return;
	}

	FVector HitLocation;
	FKalkiGridCoord ClickedCoord;

	if (RaycastToGrid(HitLocation, ClickedCoord))
	{
		// Valid tile clicked
		SelectTile(ClickedCoord);

		KalkiLog::Grid(
			FString::Printf(TEXT("Tile clicked: %s"), *ClickedCoord.ToString())
		);
	}
	else
	{
		// Clicked outside grid
		ClearSelection();

		KalkiLog::Grid(
			TEXT("Click outside grid - selection cleared"),
			EKalkiLogSeverity::Verbose
		);
	}
}

void UKalkiGridInteractionComponent::HandleRightClick()
{
	if (!bEnableClickDetection)
	{
		return;
	}

	// Right click clears selection (or could be context menu in future)
	ClearSelection();

	KalkiLog::Grid(TEXT("Right click - selection cleared"), EKalkiLogSeverity::Verbose);
}

bool UKalkiGridInteractionComponent::RaycastToGrid(FVector& OutHitLocation, FKalkiGridCoord& OutCoord)
{
	if (!OwningController || !GridManager)
	{
		return false;
	}

	// Get mouse ray
	FVector WorldLocation, WorldDirection;
	if (!OwningController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		return false;
	}

	// Perform raycast
	FHitResult HitResult;
	FVector TraceEnd = WorldLocation + (WorldDirection * MaxRaycastDistance);
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		WorldLocation,
		TraceEnd,
		RaycastChannel,
		QueryParams
	);

	if (bHit)
	{
		OutHitLocation = HitResult.Location;

		// Convert world position to grid coordinate
		OutCoord = GridManager->WorldPositionToCoord(OutHitLocation);

		// Validate coordinate
		if (GridManager->IsValidCoord(OutCoord))
		{
			return true;
		}
	}

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
	{
		KalkiLog::Grid(
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
	{
		GridVisualizer->SelectTile(Coord);
	}

	KalkiLog::Grid(
		FString::Printf(TEXT("Tile selected: %s"), *Coord.ToString())
	);
}

void UKalkiGridInteractionComponent::ClearSelection()
{
	if (!bHasSelection)
	{
		return;
	}

	bHasSelection = false;
	SelectedTile = FKalkiGridCoord::Invalid();

	// Clear visualizer
	if (GridVisualizer)
	{
		GridVisualizer->DeselectTile();
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
	{
		KalkiLog::System(
			TEXT("GridInteractionComponent - Owner is not a PlayerController!"),
			EKalkiLogSeverity::Error,
			this
		);
		return;
	}

	// Get grid manager
	GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
	if (!GridManager)
	{
		KalkiLog::System(
			TEXT("GridInteractionComponent - GridManager not found!"),
			EKalkiLogSeverity::Warning,
			this
		);
	}

	// Find grid visualizer
	// Note: This might not exist at BeginPlay, so we'll try again if needed
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AKalkiGridVisualizer::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		GridVisualizer = Cast<AKalkiGridVisualizer>(FoundActors[0]);
		
		if (GridVisualizer)
		{
			KalkiLog::System(TEXT("GridInteractionComponent - Found GridVisualizer"));
		}
	}
}

void UKalkiGridInteractionComponent::DrawDebugVisuals(const FVector& Start, const FVector& End, bool bHit, const FVector& HitLocation)
{
	if (!GetWorld())
	{
		return;
	}

	// Draw raycast line
	FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(
		GetWorld(),
		Start,
		bHit ? HitLocation : End,
		LineColor,
		false,
		0.0f,
		0,
		2.0f
	);

	// Draw hit point
	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			HitLocation,
			25.0f,
			12,
			FColor::Yellow,
			false,
			0.0f
		);
	}
}