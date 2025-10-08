// Copyright of V.S. Puranam and no one else


// Private/Grid/KalkiGridManager.cpp

#include "Grid/KalkiGridManager.h"
#include "Grid/KalkiGridTile.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "TimerManager.h"

void UKalkiGridManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    UE_LOG(LogTemp, Log, TEXT("KalkiGridManager initialized"));
}

void UKalkiGridManager::Deinitialize()
{
    // Clear timer
    if (UWorld* World = GetWorld())
    {
        World->GetTimerManager().ClearTimer(VisualizationTimerHandle);
    }

    GridTiles.Empty();

    Super::Deinitialize();
}

void UKalkiGridManager::OnWorldBeginPlay(UWorld& InWorld)
{
    Super::OnWorldBeginPlay(InWorld);

    // Set up debug visualization timer
    if (bShowGridVisualization)
    {
        InWorld.GetTimerManager().SetTimer(
            VisualizationTimerHandle,
            this,
            &UKalkiGridManager::DrawDebugGrid,
            0.1f, // Every 0.1 seconds
            true  // Loop
        );
    }
}

void UKalkiGridManager::RegisterTile(AKalkiGridTile* Tile)
{
    if (!Tile)
    {
        UE_LOG(LogTemp, Warning, TEXT("KalkiGridManager: Attempted to register null tile"));
        return;
    }

    FIntPoint GridPos = Tile->GetGridPosition();

    // Check if position is already occupied
    if (GridTiles.Contains(GridPos))
    {
        UE_LOG(LogTemp, Warning, TEXT("KalkiGridManager: Tile already exists at position (%d, %d)"), 
            GridPos.X, GridPos.Y);
        return;
    }

    GridTiles.Add(GridPos, Tile);
    
    UE_LOG(LogTemp, Verbose, TEXT("KalkiGridManager: Registered tile at (%d, %d)"), 
        GridPos.X, GridPos.Y);
}

void UKalkiGridManager::UnregisterTile(AKalkiGridTile* Tile)
{
    if (!Tile)
    {
        return;
    }

    FIntPoint GridPos = Tile->GetGridPosition();
    GridTiles.Remove(GridPos);
    
    UE_LOG(LogTemp, Verbose, TEXT("KalkiGridManager: Unregistered tile at (%d, %d)"), 
        GridPos.X, GridPos.Y);
}

AKalkiGridTile* UKalkiGridManager::GetTileAt(const FIntPoint& GridCoords) const
{
    if (const TObjectPtr<AKalkiGridTile>* TilePtr = GridTiles.Find(GridCoords))
    {
        return *TilePtr;
    }
    
    return nullptr;
}

AKalkiGridTile* UKalkiGridManager::GetTileAtWorldLocation(const FVector& WorldLocation) const
{
    FIntPoint GridCoords = WorldToGrid(WorldLocation);
    return GetTileAt(GridCoords);
}

bool UKalkiGridManager::IsValidGridPosition(const FIntPoint& GridCoords) const
{
    return GridTiles.Contains(GridCoords);
}

bool UKalkiGridManager::IsWalkable(const FIntPoint& GridCoords) const
{
    AKalkiGridTile* Tile = GetTileAt(GridCoords);
    return Tile ? Tile->IsWalkable() : false;
}

bool UKalkiGridManager::CanBeOccupied(const FIntPoint& GridCoords) const
{
    AKalkiGridTile* Tile = GetTileAt(GridCoords);
    return Tile ? Tile->CanBeOccupied() : false;
}

FIntPoint UKalkiGridManager::WorldToGrid(const FVector& WorldLocation) const
{
    // Offset from grid origin
    FVector Offset = WorldLocation - GridOrigin;
    
    // Convert to grid coordinates
    int32 X = FMath::RoundToInt(Offset.X / TileSize);
    int32 Y = FMath::RoundToInt(Offset.Y / TileSize);
    
    return FIntPoint(X, Y);
}

FVector UKalkiGridManager::GridToWorld(const FIntPoint& GridCoords) const
{
    // Calculate world position (center of tile)
    float WorldX = GridOrigin.X + (GridCoords.X * TileSize);
    float WorldY = GridOrigin.Y + (GridCoords.Y * TileSize);
    float WorldZ = GridOrigin.Z;
    
    return FVector(WorldX, WorldY, WorldZ);
}

TArray<AKalkiGridTile*> UKalkiGridManager::GetAllTiles() const
{
    TArray<AKalkiGridTile*> Tiles;
    Tiles.Reserve(GridTiles.Num());
    
    for (const auto& Pair : GridTiles)
    {
        if (Pair.Value)
        {
            Tiles.Add(Pair.Value);
        }
    }
    
    return Tiles;
}

void UKalkiGridManager::SetGridVisualizationEnabled(bool bEnabled)
{
    bShowGridVisualization = bEnabled;

    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    if (bEnabled)
    {
        // Start drawing
        World->GetTimerManager().SetTimer(
            VisualizationTimerHandle,
            this,
            &UKalkiGridManager::DrawDebugGrid,
            0.1f,
            true
        );
    }
    else
    {
        // Stop drawing
        World->GetTimerManager().ClearTimer(VisualizationTimerHandle);
    }
}

void UKalkiGridManager::ToggleGridVisualization()
{
    SetGridVisualizationEnabled(!bShowGridVisualization);
}

void UKalkiGridManager::DrawDebugGrid()
{
    UWorld* World = GetWorld();
    if (!World || !bShowGridVisualization)
    {
        return;
    }

    // Draw grid lines for each tile
    for (const auto& Pair : GridTiles)
    {
        const FIntPoint& GridPos = Pair.Key;
        AKalkiGridTile* Tile = Pair.Value;

        if (!Tile)
        {
            continue;
        }

        FVector TileCenter = GridToWorld(GridPos);
        float HalfSize = TileSize * 0.5f;

        // Four corners of the tile
        FVector Corners[4] = {
            TileCenter + FVector(-HalfSize, -HalfSize, 0),
            TileCenter + FVector(HalfSize, -HalfSize, 0),
            TileCenter + FVector(HalfSize, HalfSize, 0),
            TileCenter + FVector(-HalfSize, HalfSize, 0)
        };

        // Draw the tile outline
        FColor LineColor = Tile->IsWalkable() ? FColor::Green : FColor::Red;
        if (Tile->IsOccupied())
        {
            LineColor = FColor::Yellow;
        }

        for (int32 i = 0; i < 4; i++)
        {
            FVector Start = Corners[i];
            FVector End = Corners[(i + 1) % 4];
            DrawDebugLine(World, Start, End, LineColor, false, 0.15f, 0, 2.0f);
        }

        // Draw grid coordinates
        DrawDebugString(World, TileCenter + FVector(0, 0, 10), 
            FString::Printf(TEXT("%d,%d"), GridPos.X, GridPos.Y),
            nullptr, FColor::White, 0.15f, false, 0.8f);
    }
}

void UKalkiGridManager::SpawnDebugGrid(FVector Origin, int32 SizeX, int32 SizeY, TSubclassOf<AKalkiGridTile> TileClass)
{
    UWorld* World = GetWorld();
    if (!World)
    {
        UE_LOG(LogTemp, Error, TEXT("KalkiGridManager: Cannot spawn debug grid - no world"));
        return;
    }

    if (!TileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("KalkiGridManager: Cannot spawn debug grid - no tile class specified"));
        return;
    }

    // Clear existing tiles first
    ClearAllTiles();

    // Set grid origin
    SetGridOrigin(Origin);

    UE_LOG(LogTemp, Log, TEXT("KalkiGridManager: Spawning debug grid %dx%d at %s"), 
        SizeX, SizeY, *Origin.ToString());

    // Spawn tiles in a grid pattern
    for (int32 X = 0; X < SizeX; X++)
    {
        for (int32 Y = 0; Y < SizeY; Y++)
        {
            FIntPoint GridPos(X, Y);
            FVector WorldPos = GridToWorld(GridPos);

            // Spawn tile
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

            AKalkiGridTile* Tile = World->SpawnActor<AKalkiGridTile>(TileClass, WorldPos, FRotator::ZeroRotator, SpawnParams);
            
            if (Tile)
            {
                Tile->SetGridPosition(GridPos);
                // Tile will auto-register in BeginPlay
            }
        }
    }

    UE_LOG(LogTemp, Log, TEXT("KalkiGridManager: Debug grid spawned with %d tiles"), GetTileCount());
}

void UKalkiGridManager::ClearAllTiles()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Destroy all tile actors
    TArray<AKalkiGridTile*> TilesToDestroy = GetAllTiles();
    for (AKalkiGridTile* Tile : TilesToDestroy)
    {
        if (Tile)
        {
            Tile->Destroy();
        }
    }

    GridTiles.Empty();
    
    UE_LOG(LogTemp, Log, TEXT("KalkiGridManager: Cleared all tiles"));
}
