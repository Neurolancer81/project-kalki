// Copyright of V.S. Puranam and no one else

// Private/Grid/KalkiGridVisualizer.cpp

#include "Grid/KalkiGridVisualizer.h"

#include "Blueprint/UserWidget.h"
#include "Grid/KalkiGridManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Logging/KalkiLog.h"

AKalkiGridVisualizer::AKalkiGridVisualizer()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    // Create instanced mesh component
    TileInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("TileInstances"));
    RootComponent = TileInstances;

    // Set up instancing
    TileInstances->SetCastShadow(false); // Grid doesn't need shadows
    TileInstances->SetCollisionEnabled(ECollisionEnabled::NoCollision); // Visual only
    TileInstances->SetMobility(EComponentMobility::Movable); // Allow runtime changes
    TileInstances->NumCustomDataFloats = 4; // Enable custom data for per-instance colors (RGBA)

    // Don't replicate (local visuals only)
    bReplicates = false;
    AActor::SetReplicateMovement(false);
    
}

void AKalkiGridVisualizer::BeginPlay()
{
    Super::BeginPlay();

    KalkiLog::Grid(TEXT("GridVisualizer::BeginPlay - START"), EKalkiLogSeverity::Log, this);

    // Get grid manager
    GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::Grid(TEXT("GridVisualizer - Failed to get GridManager"), EKalkiLogSeverity::Error, this);
        return;
    }

    KalkiLog::Grid(TEXT("GridVisualizer - GridManager obtained"), EKalkiLogSeverity::Log, this);

    // Bind to tile changed event
    KalkiLog::Grid(TEXT("GridVisualizer - Binding to OnTileChanged event"), EKalkiLogSeverity::Log, this);
    GridManager->OnTileChanged.AddDynamic(this, &AKalkiGridVisualizer::OnTileChanged);
    KalkiLog::Grid(TEXT("GridVisualizer - OnTileChanged event bound"), EKalkiLogSeverity::Log, this);

    // Wait a frame for grid to be created, then initialize
    FTimerHandle InitTimer;
    GetWorld()->GetTimerManager().SetTimer(InitTimer, this, &AKalkiGridVisualizer::InitializeGrid, 0.1f, false);

    KalkiLog::Grid(TEXT("GridVisualizer::BeginPlay - END"), EKalkiLogSeverity::Log, this);
}

void AKalkiGridVisualizer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Unbind from events
    if (GridManager)
    {
        GridManager->OnTileChanged.RemoveDynamic(this, &AKalkiGridVisualizer::OnTileChanged);
    }

    // Clean up widget
    if (GridInfoWidget)
    {
        GridInfoWidget->RemoveFromParent();
        GridInfoWidget = nullptr;
    }

    // Clean up ViewModel
    GridInfoViewModel = nullptr;

    Super::EndPlay(EndPlayReason);
}

void AKalkiGridVisualizer::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bGridInitialized || !bEnableHover)
    {
        return;
    }

    UpdateHover(DeltaTime);
}

void AKalkiGridVisualizer::InitializeGrid()
{
    if (!GridManager || !TileInstances)
    {
        KalkiLog::Grid(TEXT("GridVisualizer - GridManager or TileInstances is null"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Check if grid exists
    if (GridManager->GetGridSizeX() == 0 || GridManager->GetGridSizeY() == 0)
    {
        KalkiLog::Grid(TEXT("GridVisualizer - Grid not created yet, skipping initialization"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Set mesh if available
    if (TileMesh)
    {
        TileInstances->SetStaticMesh(TileMesh);
        KalkiLog::Grid(TEXT("GridVisualizer - Tile mesh set"), EKalkiLogSeverity::Log, this);
    }
    else
    {
        KalkiLog::Grid(TEXT("GridVisualizer - TileMesh not set in Blueprint, instances will be invisible"), EKalkiLogSeverity::Warning, this);
    }

    // Set material if available
    if (TileMaterial)
    {
        DynamicTileMaterial = UMaterialInstanceDynamic::Create(TileMaterial, this);
        TileInstances->SetMaterial(0, DynamicTileMaterial);
        KalkiLog::Grid(TEXT("GridVisualizer - Tile material set"), EKalkiLogSeverity::Log, this);
    }
    else
    {
        KalkiLog::Grid(TEXT("GridVisualizer - TileMaterial not set, using default material"), EKalkiLogSeverity::Warning, this);
    }

    // Clear any existing instances
    TileInstances->ClearInstances();

    // Create instance for each tile
    int32 GridSizeX = GridManager->GetGridSizeX();
    int32 GridSizeY = GridManager->GetGridSizeY();

    for (int32 X = 0; X < GridSizeX; ++X)
    {
        for (int32 Y = 0; Y < GridSizeY; ++Y)
        {
            FKalkiGridCoord Coord(X, Y);
            FKalkiGridTile Tile = GridManager->GetTile(Coord);

            // Get world position and add Z offset
            FVector WorldPos = Tile.WorldPosition;
            WorldPos.Z += TileZOffset;

            // ⭐ Create transform with scale
            FVector Scale(TileScale, TileScale, 1.0f); // Scale X and Y, keep Z at 1.0
            FTransform TileTransform(FRotator::ZeroRotator, WorldPos, Scale);

            // Add instance
            int32 InstanceIndex = TileInstances->AddInstance(TileTransform, true);

            // Set per-instance color (custom data)
            FLinearColor TileColor = GetTileColor(Tile);
            TileInstances->SetCustomDataValue(InstanceIndex, 0, TileColor.R, true);
            TileInstances->SetCustomDataValue(InstanceIndex, 1, TileColor.G, true);
            TileInstances->SetCustomDataValue(InstanceIndex, 2, TileColor.B, true);
            TileInstances->SetCustomDataValue(InstanceIndex, 3, TileColor.A, true);
        }
    }

    bGridInitialized = true;

    KalkiLog::Grid(
        FString::Printf(TEXT("GridVisualizer initialized: %d tiles, Mesh=%s, Material=%s"), 
            GridSizeX * GridSizeY,
            TileMesh ? TEXT("Set") : TEXT("NULL"),
            TileMaterial ? TEXT("Set") : TEXT("NULL")),
        EKalkiLogSeverity::Log,
        this
    );

    // Around line 150-160, replace the widget creation code with:

    // Create ViewModel
    GridInfoViewModel = NewObject<UKalkiGridInfoViewModel>(this);
    if (GridInfoViewModel)
    {
        KalkiLog::Grid(TEXT("GridVisualizer - ViewModel created"), EKalkiLogSeverity::Log, this);
    }

    // Create info widget if class is set
    if (GridInfoWidgetClass && !GridInfoWidget)
    {
        if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
        {
            GridInfoWidget = CreateWidget<UKalkiGridInfoWidget>(PC, GridInfoWidgetClass);
            if (GridInfoWidget)
            {
                GridInfoWidget->AddToViewport(100); // High Z-order
                GridInfoWidget->SetVisibility(ESlateVisibility::Hidden); // Start hidden
            
                // Set the ViewModel on the widget
                GridInfoWidget->SetViewModel(GridInfoViewModel);
            
                KalkiLog::Grid(TEXT("GridVisualizer - Info widget created and ViewModel connected"), EKalkiLogSeverity::Log, this);
            }
        }
    }

    // Start with grid visibility based on settings
    SetGridVisible(bShowGrid);
}

void AKalkiGridVisualizer::RefreshGrid()
{
    if (!bGridInitialized || !GridManager || !TileInstances)
    {
        return;
    }

    int32 GridSizeX = GridManager->GetGridSizeX();
    int32 GridSizeY = GridManager->GetGridSizeY();
    int32 InstanceIndex = 0;

    for (int32 X = 0; X < GridSizeX; ++X)
    {
        for (int32 Y = 0; Y < GridSizeY; ++Y)
        {
            FKalkiGridCoord Coord(X, Y);
            FKalkiGridTile Tile = GridManager->GetTile(Coord);

            // Update color
            FLinearColor TileColor = GetTileColor(Tile);
            TileInstances->SetCustomDataValue(InstanceIndex, 0, TileColor.R, true);
            TileInstances->SetCustomDataValue(InstanceIndex, 1, TileColor.G, true);
            TileInstances->SetCustomDataValue(InstanceIndex, 2, TileColor.B, true);
            TileInstances->SetCustomDataValue(InstanceIndex, 3, TileColor.A, true);

            InstanceIndex++;
        }
    }

    // Force update
    TileInstances->MarkRenderStateDirty();
}

void AKalkiGridVisualizer::UpdateTileVisual(const FKalkiGridCoord& Coord)
{
    if (!bGridInitialized || !GridManager || !TileInstances)
    {
        return;
    }

    // Calculate instance index (X * GridSizeY + Y)
    int32 GridSizeY = GridManager->GetGridSizeY();
    int32 InstanceIndex = Coord.X * GridSizeY + Coord.Y;

    FKalkiGridTile Tile = GridManager->GetTile(Coord);
    FLinearColor TileColor = GetTileColor(Tile);

    TileInstances->SetCustomDataValue(InstanceIndex, 0, TileColor.R, true);
    TileInstances->SetCustomDataValue(InstanceIndex, 1, TileColor.G, true);
    TileInstances->SetCustomDataValue(InstanceIndex, 2, TileColor.B, true);
    TileInstances->SetCustomDataValue(InstanceIndex, 3, TileColor.A, true);

    TileInstances->MarkRenderStateDirty();
}

FLinearColor AKalkiGridVisualizer::GetTileColor(const FKalkiGridTile& Tile) const
{
    FLinearColor FinalColor = WalkableColor;

    // Check walkability first
    if (bShowWalkability)
    {
        if (Tile.IsOccupied())
        {
            FinalColor = OccupiedColor;
        }
        else if (!Tile.bWalkable)
        {
            FinalColor = UnwalkableColor;
        }
        else
        {
            FinalColor = WalkableColor;
        }
    }

    // Blend with elevation color
    if (bShowElevation && MaxElevationForColor > 0.0f)
    {
        float ElevationPercent = FMath::Clamp(Tile.Elevation / MaxElevationForColor, 0.0f, 1.0f);
        FLinearColor ElevationColor = FMath::Lerp(BaseElevationColor, HighElevationColor, ElevationPercent);

        // Blend walkability color with elevation (50/50 mix)
        FinalColor = FMath::Lerp(FinalColor, ElevationColor, 0.5f);
    }

    return FinalColor;
}

void AKalkiGridVisualizer::UpdateHover(float DeltaTime)
{
    // Get tile under cursor
    FKalkiGridCoord NewHoveredTile(-1, -1);
    bool bIsHovering = GetTileUnderCursor(NewHoveredTile);

    // Check if hover changed
    if (NewHoveredTile != HoveredTile)
    {
        // Clear previous hover highlight
        if (HoveredTile.X >= 0 && HoveredTile.Y >= 0)
        {
            ClearTileHighlight(HoveredTile);
        }

        // Update hover state
        PreviousHoveredTile = HoveredTile;
        HoveredTile = NewHoveredTile;

        // Apply new hover highlight
        if (bIsHovering && HoveredTile.X >= 0 && HoveredTile.Y >= 0)
        {
            SetTileHighlight(HoveredTile, HoverColor);
        }

        // Update info widget visibility and data
        if (GridInfoWidget && GridInfoViewModel)
        {
            if (bIsHovering)
            {
                // Update ViewModel with new tile data
                FKalkiGridTile Tile = GridManager->GetTile(HoveredTile);
                GridInfoViewModel->UpdateFromTile(HoveredTile, Tile);
                
                // Show widget
                GridInfoWidget->SetVisibility(ESlateVisibility::Visible);
            }
            else
            {
                // Hide widget
                GridInfoWidget->SetVisibility(ESlateVisibility::Hidden);
                
                // Clear ViewModel
                GridInfoViewModel->Clear();
            }
        }
    }

    // ⭐ NEW: Update widget position to follow mouse (every frame)
    if (GridInfoWidget && GridInfoWidget->GetVisibility() == ESlateVisibility::Visible)
    {
        UpdateWidgetPosition();
    }
}

bool AKalkiGridVisualizer::GetTileUnderCursor(FKalkiGridCoord& OutCoord) const
{
    if (!GridManager)
    {
        return false;
    }

    // Get player controller
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        return false;
    }

    // Get mouse position
    FVector MouseWorldLocation, MouseWorldDirection;
    if (!PC->DeprojectMousePositionToWorld(MouseWorldLocation, MouseWorldDirection))
    {
        return false;
    }

    // Raycast from mouse to world
    FVector Start = MouseWorldLocation;
    FVector End = Start + (MouseWorldDirection * HoverRaycastDistance);

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    // Raycast against world geometry
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams))
    {
        // Convert hit location to grid coordinate
        FVector HitLocation = HitResult.Location;
        OutCoord = GridManager->WorldPositionToCoord(HitLocation);

        // Validate coordinate
        if (GridManager->IsValidCoord(OutCoord))
        {
            return true;
        }
    }

    return false;
}

void AKalkiGridVisualizer::SetTileHighlight(const FKalkiGridCoord& Coord, const FLinearColor& Color)
{
    if (!bGridInitialized || !GridManager || !TileInstances)
    {
        return;
    }

    if (!GridManager->IsValidCoord(Coord))
    {
        return;
    }

    // Calculate instance index
    int32 GridSizeY = GridManager->GetGridSizeY();
    int32 InstanceIndex = Coord.X * GridSizeY + Coord.Y;

    // Get base tile color
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
    FLinearColor BaseColor = GetTileColor(Tile);

    // Blend highlight color with base color
    FLinearColor FinalColor = FLinearColor::LerpUsingHSV(BaseColor, Color, 0.5f);

    // Set color
    TileInstances->SetCustomDataValue(InstanceIndex, 0, FinalColor.R, true);
    TileInstances->SetCustomDataValue(InstanceIndex, 1, FinalColor.G, true);
    TileInstances->SetCustomDataValue(InstanceIndex, 2, FinalColor.B, true);
    TileInstances->SetCustomDataValue(InstanceIndex, 3, FinalColor.A, true);

    TileInstances->MarkRenderStateDirty();
}

void AKalkiGridVisualizer::ClearTileHighlight(const FKalkiGridCoord& Coord)
{
    // Restore original tile color
    UpdateTileVisual(Coord);
}

void AKalkiGridVisualizer::ClearAllHighlights()
{
    // Refresh entire grid to clear all highlights
    RefreshGrid();
}

void AKalkiGridVisualizer::SelectTile(const FKalkiGridCoord& Coord)
{
    if (!GridManager || !GridManager->IsValidCoord(Coord))
    {
        return;
    }

    // Clear previous selection
    if (bHasTileSelected)
    {
        ClearTileHighlight(SelectedTile);
    }

    // Set new selection
    SelectedTile = Coord;
    bHasTileSelected = true;

    // Highlight selected tile
    SetTileHighlight(SelectedTile, SelectedColor);

    KalkiLog::Grid(
        FString::Printf(TEXT("Tile selected: (%d, %d)"), Coord.X, Coord.Y),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiGridVisualizer::DeselectTile()
{
    if (!bHasTileSelected)
    {
        return;
    }

    // Clear selection highlight
    ClearTileHighlight(SelectedTile);

    // Clear movement range
    HideMovementRange();

    bHasTileSelected = false;
    SelectedTile = FKalkiGridCoord(-1, -1);

    KalkiLog::Grid(TEXT("Tile deselected"), EKalkiLogSeverity::Log, this);
}

void AKalkiGridVisualizer::ShowMovementRange(const FKalkiGridCoord& Origin, int32 Range)
{
    if (!GridManager)
    {
        return;
    }

    // Clear previous range
    HideMovementRange();

    // Get tiles in range using existing function
    TArray<FKalkiGridCoord> RangeTilesArray = GridManager->GetTilesInRange(Origin, Range, false);
    
    // Convert TArray to TSet for faster lookups
    MovementRangeTiles = TSet<FKalkiGridCoord>(RangeTilesArray);

    // Highlight all tiles in range
    for (const FKalkiGridCoord& Coord : MovementRangeTiles)
    {
        // Don't highlight the origin tile
        if (Coord == Origin)
        {
            continue;
        }

        SetTileHighlight(Coord, MovementRangeColor);
    }

    KalkiLog::Grid(
        FString::Printf(TEXT("Movement range shown: %d tiles from (%d, %d)"), 
            MovementRangeTiles.Num(), Origin.X, Origin.Y),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiGridVisualizer::HideMovementRange()
{
    if (MovementRangeTiles.Num() == 0)
    {
        return;
    }

    // Clear highlights for all range tiles
    for (const FKalkiGridCoord& Coord : MovementRangeTiles)
    {
        ClearTileHighlight(Coord);
    }

    MovementRangeTiles.Empty();

    KalkiLog::Grid(TEXT("Movement range hidden"), EKalkiLogSeverity::Log, this);
}

void AKalkiGridVisualizer::OnTileChanged(const FKalkiGridCoord& Coord)
{
    if (!bGridInitialized)
    {
        KalkiLog::Grid(TEXT("OnTileChanged called before grid initialized"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Update just this one tile
    UpdateTileVisual(Coord);
}

void AKalkiGridVisualizer::SetGridVisible(bool bVisible)
{
    if (TileInstances)
    {
        TileInstances->SetVisibility(bVisible);
        
        KalkiLog::Grid(
            FString::Printf(TEXT("Grid visibility: %s"), bVisible ? TEXT("Visible") : TEXT("Hidden")),
            EKalkiLogSeverity::Log,
            this
        );
    }
}

void AKalkiGridVisualizer::UpdateWidgetPosition()
{
    if (!GridInfoWidget)
    {
        return;
    }

    // Get player controller
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (!PC)
    {
        return;
    }

    // Get mouse position in viewport
    float MouseX, MouseY;
    if (PC->GetMousePosition(MouseX, MouseY))
    {
        // Get viewport size
        FVector2D ViewportSize;
        if (GEngine && GEngine->GameViewport)
        {
            GEngine->GameViewport->GetViewportSize(ViewportSize);
        }

        // Get widget desired size
        FVector2D WidgetSize = GridInfoWidget->GetDesiredSize();

        // Calculate position with offset
        FVector2D WidgetPosition = FVector2D(MouseX, MouseY) + TooltipOffset;

        // Clamp to viewport bounds (prevent widget going off-screen)
        // Right edge
        if (WidgetPosition.X + WidgetSize.X > ViewportSize.X)
        {
            WidgetPosition.X = MouseX - WidgetSize.X - TooltipOffset.X; // Show on left of cursor instead
        }

        // Bottom edge
        if (WidgetPosition.Y + WidgetSize.Y > ViewportSize.Y)
        {
            WidgetPosition.Y = MouseY - WidgetSize.Y - TooltipOffset.Y; // Show above cursor instead
        }

        // Top-left edge (ensure widget stays on screen)
        WidgetPosition.X = FMath::Max(0.0f, WidgetPosition.X);
        WidgetPosition.Y = FMath::Max(0.0f, WidgetPosition.Y);

        // Set widget position
        GridInfoWidget->SetPositionInViewport(WidgetPosition, false);
    }
}