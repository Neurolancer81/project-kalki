// Copyright of V.S. Puranam and no one else

// Private/Debug/KalkiCheatManager.cpp

#include "Debug/KalkiCheatManager.h"

#include "EngineUtils.h"
#include "Logging/KalkiLog.h"
#include "Logging/KalkiLogSubsystem.h"
#include "UI/Common/KalkiHUD.h"
#include "Grid/KalkiGridManager.h"
#include "Grid/KalkiGridTypes.h"
#include "Grid/KalkiGridVisualizer.h"
#include "Level/KalkiLevelManager.h"

// === Combat Log Testing ===

void UKalkiCheatManager::StartTestCombatLog()
{
    UKalkiLogSubsystem* LogSys = GetWorld()->GetSubsystem<UKalkiLogSubsystem>();
    if (!LogSys)
    {
        KalkiLog::System(TEXT("Failed to get LogSubsystem"), EKalkiLogSeverity::Error);
        return;
    }

    static int32 TestCombatCounter = 1;
    FString CombatName = FString::Printf(TEXT("Test Combat %d"), TestCombatCounter++);
    
    // FIXED: Use correct function name
    LogSys->StartCombatSession(CombatName);

    // Log some test messages
    KalkiLog::Combat(TEXT("Combat started!"));
    KalkiLog::Movement(TEXT("Arjuna moves from (2,4) to (5,7)"));
    KalkiLog::Combat(TEXT("Arjuna attacks Goblin - Hit! 8 damage"));
    KalkiLog::Combat(TEXT("Goblin takes 8 damage"), EKalkiLogSeverity::Warning);
    KalkiLog::Combat(TEXT("Goblin dies!"), EKalkiLogSeverity::Error);
}

void UKalkiCheatManager::EndTestCombatLog()
{
    UKalkiLogSubsystem* LogSys = GetWorld()->GetSubsystem<UKalkiLogSubsystem>();
    if (!LogSys)
    {
        return;
    }

    KalkiLog::System(TEXT("Combat complete"));
    
    // FIXED: Use correct function name
    LogSys->EndCombatSession();
}

void UKalkiCheatManager::LogTestMessage(const FString& Message)
{
    KalkiLog::Combat(Message);
}

// === UI Testing ===

void UKalkiCheatManager::ShowCombatUI()
{
    AKalkiHUD* KalkiHUD = Cast<AKalkiHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (KalkiHUD)
    {
        KalkiHUD->SwitchToCombatMode();
    }
}

void UKalkiCheatManager::ShowStrategyUI()
{
    AKalkiHUD* KalkiHUD = Cast<AKalkiHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (KalkiHUD)
    {
        KalkiHUD->SwitchToStrategyMode();
    }
}

void UKalkiCheatManager::ToggleUIMode()
{
    AKalkiHUD* KalkiHUD = Cast<AKalkiHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
    if (KalkiHUD)
    {
        if (KalkiHUD->GetCurrentMode() == EKalkiGameMode::Combat)
        {
            KalkiHUD->SwitchToStrategyMode();
        }
        else
        {
            KalkiHUD->SwitchToCombatMode();
        }
    }
}

// === Grid Testing ===

void UKalkiCheatManager::CreateTestGrid(int32 SizeX, int32 SizeY)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    GridManager->CreateGrid(SizeX, SizeY);
    KalkiLog::System(FString::Printf(TEXT("Test grid created: %dx%d"), SizeX, SizeY));
}

void UKalkiCheatManager::PrintGridInfo()
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    KalkiLog::System(
        FString::Printf(TEXT("Grid: %dx%d, TileSize=%.1f, Origin=%s"),
            GridManager->GetGridSizeX(),
            GridManager->GetGridSizeY(),
            GridManager->GetTileSize(),
            *GridManager->GetGridOrigin().ToString())
    );
}

void UKalkiCheatManager::PrintTileInfo(int32 X, int32 Y)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    FKalkiGridCoord Coord(X, Y);
    if (!GridManager->IsValidCoord(Coord))
    {
        KalkiLog::System(FString::Printf(TEXT("Invalid coordinate: (%d, %d)"), X, Y), EKalkiLogSeverity::Error);
        return;
    }

    FKalkiGridTile Tile = GridManager->GetTile(Coord);
    
    KalkiLog::System(
        FString::Printf(TEXT("Tile (%d, %d): WorldPos=%s, Elevation=%.1f, Walkable=%s, Occupied=%s"),
            X, Y,
            *Tile.WorldPosition.ToString(),
            Tile.Elevation,
            Tile.bWalkable ? TEXT("Yes") : TEXT("No"),
            Tile.IsOccupied() ? TEXT("Yes") : TEXT("No"))
    );
}

void UKalkiCheatManager::SetTileElevation(int32 X, int32 Y, float Elevation)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    FKalkiGridCoord Coord(X, Y);
    if (GridManager->SetElevation(Coord, Elevation))
    {
        KalkiLog::Grid(FString::Printf(TEXT("Set tile (%d, %d) elevation to %.1f"), X, Y, Elevation));
    }
    else
    {
        KalkiLog::Grid(FString::Printf(TEXT("Failed to set tile (%d, %d) elevation"), X, Y), EKalkiLogSeverity::Error);
    }
}

void UKalkiCheatManager::SetTileWalkable(int32 X, int32 Y, bool bWalkable)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::Grid(TEXT("SetTileWalkable - GridManager not found"), EKalkiLogSeverity::Error);
        return;
    }

    FKalkiGridCoord Coord(X, Y);
    
    if (!GridManager->IsValidCoord(Coord))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("SetTileWalkable - Invalid coord: (%d, %d)"), X, Y), 
            EKalkiLogSeverity::Error
        );
        return;
    }

    // Get the tile
    FKalkiGridTile Tile = GridManager->GetTile(Coord);
    
    // Modify walkability
    Tile.bWalkable = bWalkable;
    
    // Set it back (this will trigger OnTileChanged event)
    if (GridManager->SetTile(Coord, Tile))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Set tile (%d, %d) walkable: %s"), 
                X, Y, bWalkable ? TEXT("Yes") : TEXT("No"))
        );
    }
    else
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Failed to set tile (%d, %d) walkable"), X, Y), 
            EKalkiLogSeverity::Error
        );
    }
}

void UKalkiCheatManager::CreateTestPlatform(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float Elevation)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    int32 TilesSet = 0;
    for (int32 X = StartX; X <= EndX; ++X)
    {
        for (int32 Y = StartY; Y <= EndY; ++Y)
        {
            if (GridManager->SetElevation(FKalkiGridCoord(X, Y), Elevation))
            {
                TilesSet++;
            }
        }
    }

    KalkiLog::Grid(
        FString::Printf(TEXT("Created platform (%d,%d) to (%d,%d) at elevation %.1f (%d tiles)"),
            StartX, StartY, EndX, EndY, Elevation, TilesSet)
    );
}

void UKalkiCheatManager::CreateTestRamp(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float StartElevation, float EndElevation)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    // Calculate ramp
    int32 DeltaX = EndX - StartX;
    int32 DeltaY = EndY - StartY;
    bool bXAxisLonger = FMath::Abs(DeltaX) >= FMath::Abs(DeltaY);
    int32 Steps = bXAxisLonger ? FMath::Abs(DeltaX) : FMath::Abs(DeltaY);
    
    if (Steps == 0)
    {
        GridManager->SetElevation(FKalkiGridCoord(StartX, StartY), StartElevation);
        return;
    }

    float ElevationStep = (EndElevation - StartElevation) / Steps;

    int32 TilesSet = 0;
    for (int32 i = 0; i <= Steps; ++i)
    {
        float T = static_cast<float>(i) / Steps;
        int32 X = StartX + FMath::RoundToInt(DeltaX * T);
        int32 Y = StartY + FMath::RoundToInt(DeltaY * T);
        float Elevation = StartElevation + (ElevationStep * i);

        if (GridManager->SetElevation(FKalkiGridCoord(X, Y), Elevation))
        {
            TilesSet++;
        }
    }

    KalkiLog::Grid(
        FString::Printf(TEXT("Created ramp from (%d,%d) to (%d,%d), elevation %.1f to %.1f (%d tiles)"),
            StartX, StartY, EndX, EndY, StartElevation, EndElevation, TilesSet)
    );
}

void UKalkiCheatManager::PrintNeighbors(int32 X, int32 Y, bool bDiagonalAllowed)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    FKalkiGridCoord Coord(X, Y);
    TArray<FKalkiGridCoord> Neighbors = GridManager->GetNeighbors(Coord, bDiagonalAllowed);
    TArray<FKalkiGridCoord> WalkableNeighbors = GridManager->GetWalkableNeighbors(Coord, bDiagonalAllowed);

    KalkiLog::System(
        FString::Printf(TEXT("Tile (%d, %d) has %d neighbors (%d walkable):"),
            X, Y, Neighbors.Num(), WalkableNeighbors.Num())
    );

    for (const FKalkiGridCoord& Neighbor : Neighbors)
    {
        bool bWalkable = WalkableNeighbors.Contains(Neighbor);
        float Elevation = GridManager->GetElevation(Neighbor);
        KalkiLog::System(
            FString::Printf(TEXT("  %s - Elevation: %.1f %s"),
                *Neighbor.ToString(),
                Elevation,
                bWalkable ? TEXT("[Walkable]") : TEXT("[Blocked]"))
        );
    }
}

void UKalkiCheatManager::PrintTilesInRange(int32 X, int32 Y, int32 Range)
{
    UKalkiGridManager* GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("Failed to get GridManager"), EKalkiLogSeverity::Error);
        return;
    }

    FKalkiGridCoord Center(X, Y);
    TArray<FKalkiGridCoord> TilesInRange = GridManager->GetTilesInRange(Center, Range, false);

    KalkiLog::System(
        FString::Printf(TEXT("Tiles within range %d of (%d, %d): %d tiles"),
            Range, X, Y, TilesInRange.Num())
    );

    // Print first 10 tiles (to avoid spam)
    int32 PrintCount = FMath::Min(10, TilesInRange.Num());
    for (int32 i = 0; i < PrintCount; ++i)
    {
        const FKalkiGridCoord& Tile = TilesInRange[i];
        int32 Distance = Tile.DistanceTo(Center);
        KalkiLog::System(
            FString::Printf(TEXT("  %s - Distance: %d"), *Tile.ToString(), Distance)
        );
    }

    if (TilesInRange.Num() > 10)
    {
        KalkiLog::System(FString::Printf(TEXT("  ... and %d more"), TilesInRange.Num() - 10));
    }
}

void UKalkiCheatManager::ShowGridVisualizer()
{
    // Find LevelManager in world
    for (TActorIterator<AKalkiLevelManager> It(GetWorld()); It; ++It)
    {
        AKalkiLevelManager* LevelManager = *It;
        if (LevelManager)
        {
            LevelManager->ShowGridVisualizer();
            KalkiLog::Grid(TEXT("Grid visualizer shown"));
            return;
        }
    }
    
    KalkiLog::Grid(TEXT("No LevelManager found"), EKalkiLogSeverity::Warning);
}

void UKalkiCheatManager::HideGridVisualizer()
{
    for (TActorIterator<AKalkiLevelManager> It(GetWorld()); It; ++It)
    {
        if (AKalkiLevelManager* LevelManager = *It)
        {
            LevelManager->HideGridVisualizer();
            KalkiLog::Grid(TEXT("Grid visualizer hidden"));
            return;
        }
    }
    
    KalkiLog::Grid(TEXT("No LevelManager found"), EKalkiLogSeverity::Warning);
}

void UKalkiCheatManager::SelectGridTile(int32 X, int32 Y)
{
    // Find grid visualizer in world
    for (TActorIterator<AKalkiGridVisualizer> It(GetWorld()); It; ++It)
    {
        AKalkiGridVisualizer* Visualizer = *It;
        if (Visualizer)
        {
            FKalkiGridCoord Coord(X, Y);
            Visualizer->SelectTile(Coord);
            
            KalkiLog::Grid(
                FString::Printf(TEXT("Selected grid tile: (%d, %d)"), X, Y)
            );
            return;
        }
    }
    
    KalkiLog::Grid(TEXT("No GridVisualizer found in level"), EKalkiLogSeverity::Warning);
}

void UKalkiCheatManager::DeselectGridTile()
{
    // Find grid visualizer in world
    for (TActorIterator<AKalkiGridVisualizer> It(GetWorld()); It; ++It)
    {
        AKalkiGridVisualizer* Visualizer = *It;
        if (Visualizer)
        {
            Visualizer->DeselectTile();
            
            KalkiLog::Grid(TEXT("Deselected grid tile"));
            return;
        }
    }
    
    KalkiLog::Grid(TEXT("No GridVisualizer found in level"), EKalkiLogSeverity::Warning);
}

void UKalkiCheatManager::ShowGridMovementRange(int32 X, int32 Y, int32 Range)
{
    // Find grid visualizer in world
    for (TActorIterator<AKalkiGridVisualizer> It(GetWorld()); It; ++It)
    {
        AKalkiGridVisualizer* Visualizer = *It;
        if (Visualizer)
        {
            FKalkiGridCoord Coord(X, Y);
            Visualizer->ShowMovementRange(Coord, Range);
            
            KalkiLog::Grid(
                FString::Printf(TEXT("Showing movement range: %d tiles from (%d, %d)"), Range, X, Y)
            );
            return;
        }
    }
    
    KalkiLog::Grid(TEXT("No GridVisualizer found in level"), EKalkiLogSeverity::Warning);
}

void UKalkiCheatManager::HideGridMovementRange()
{
    // Find grid visualizer in world
    for (TActorIterator<AKalkiGridVisualizer> It(GetWorld()); It; ++It)
    {
        AKalkiGridVisualizer* Visualizer = *It;
        if (Visualizer)
        {
            Visualizer->HideMovementRange();
            
            KalkiLog::Grid(TEXT("Movement range hidden"));
            return;
        }
    }
    
    KalkiLog::Grid(TEXT("No GridVisualizer found in level"), EKalkiLogSeverity::Warning);
}

void UKalkiCheatManager::SetGridTileScale(float Scale)
{
    // Clamp to reasonable values
    Scale = FMath::Clamp(Scale, 0.1f, 1.0f);
    
    for (TActorIterator<AKalkiGridVisualizer> It(GetWorld()); It; ++It)
    {
        AKalkiGridVisualizer* Visualizer = *It;
        if (Visualizer)
        {
            // This would require exposing TileScale and recreating grid
            // Easier to just set it in Blueprint before PIE
            KalkiLog::Grid(
                FString::Printf(TEXT("SetGridTileScale: %.2f (requires grid recreation)"), Scale)
            );
            return;
        }
    }
}