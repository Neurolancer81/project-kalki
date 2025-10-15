// Copyright of V.S. Puranam and no one else

// Private/Level/KalkiLevelManager.cpp

#include "Level/KalkiLevelManager.h"
#include "Grid/KalkiGridManager.h"
#include "Grid/KalkiGridTypes.h"
#include "Logging/KalkiLog.h"
#include "Net/UnrealNetwork.h"

AKalkiLevelManager::AKalkiLevelManager()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // Enable replication
    bReplicates = true;
    bAlwaysRelevant = true; // Always replicate to all clients
    NetUpdateFrequency = 1.0f; // Low frequency since grid config rarely changes
}

void AKalkiLevelManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicate grid configuration to all clients
    DOREPLIFETIME(AKalkiLevelManager, GridSizeX);
    DOREPLIFETIME(AKalkiLevelManager, GridSizeY);
    DOREPLIFETIME(AKalkiLevelManager, TileSize);
    DOREPLIFETIME(AKalkiLevelManager, GridOrigin);
    DOREPLIFETIME(AKalkiLevelManager, bAutoCreateGrid);
    DOREPLIFETIME(AKalkiLevelManager, bUseElevation);
    DOREPLIFETIME(AKalkiLevelManager, MaxClimbHeight);
    DOREPLIFETIME(AKalkiLevelManager, MaxDropHeight);
}

bool AKalkiLevelManager::IsNetRelevantFor(const AActor* RealViewer, const AActor* ViewTarget, const FVector& SrcLocation) const
{
    // Always relevant for all players
    return true;
}

void AKalkiLevelManager::BeginPlay()
{
    Super::BeginPlay();

    // Get grid manager
    GridManager = GetWorld()->GetSubsystem<UKalkiGridManager>();
    if (!GridManager)
    {
        KalkiLog::System(TEXT("LevelManager - Failed to get GridManager"), EKalkiLogSeverity::Error, this);
        return;
    }

    // Only server creates grid (clients will build deterministically from replicated config)
    // In standalone or as server
    if (HasAuthority())
    {
        if (bAutoCreateGrid)
        {
            InitializeLevel();
        }
    }

    KalkiLog::System(
        FString::Printf(TEXT("LevelManager initialized [Role: %s]"), 
            HasAuthority() ? TEXT("Server") : TEXT("Client")),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiLevelManager::InitializeLevel()
{
    // Only server initializes
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("InitializeLevel called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    KalkiLog::System(TEXT("Initializing level..."), EKalkiLogSeverity::Log, this);

    // Create grid
    CreateGrid();

    // Future: Spawn characters, set up encounters, etc.
}

void AKalkiLevelManager::CreateGrid()
{
    // Only server creates grid
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("CreateGrid called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (!GridManager)
    {
        KalkiLog::System(TEXT("LevelManager - Cannot create grid, GridManager is null"), EKalkiLogSeverity::Error, this);
        return;
    }

    GridManager->CreateGrid(GridSizeX, GridSizeY, TileSize, GridOrigin);

    KalkiLog::System(
        FString::Printf(TEXT("Grid created: %dx%d tiles [Server]"), GridSizeX, GridSizeY),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiLevelManager::SetTileElevation(int32 X, int32 Y, float Elevation)
{
    // Only server can modify
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("SetTileElevation called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (!GridManager)
    {
        return;
    }

    FKalkiGridCoord Coord(X, Y);
    if (GridManager->SetElevation(Coord, Elevation))
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Set tile (%d, %d) elevation to %.1f"), X, Y, Elevation)
        );
    }
    else
    {
        KalkiLog::Grid(
            FString::Printf(TEXT("Failed to set tile (%d, %d) elevation"), X, Y),
            EKalkiLogSeverity::Error
        );
    }
}

void AKalkiLevelManager::CreatePlatform(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float Elevation)
{
    // Only server can modify
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("CreatePlatform called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (!GridManager)
    {
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

void AKalkiLevelManager::CreateRamp(int32 StartX, int32 StartY, int32 EndX, int32 EndY, float StartElevation, float EndElevation)
{
    // Only server can modify
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("CreateRamp called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (!GridManager)
    {
        return;
    }

    // Calculate ramp direction and length
    int32 DeltaX = EndX - StartX;
    int32 DeltaY = EndY - StartY;
    
    // Determine primary axis (longer distance)
    bool bXAxisLonger = FMath::Abs(DeltaX) >= FMath::Abs(DeltaY);
    int32 Steps = bXAxisLonger ? FMath::Abs(DeltaX) : FMath::Abs(DeltaY);
    
    if (Steps == 0)
    {
        // Single tile, just set to start elevation
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