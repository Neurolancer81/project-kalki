// Copyright of V.S. Puranam and no one else

// Private/Level/KalkiLevelManager.cpp

#include "Level/KalkiLevelManager.h"
#include "Grid/KalkiGridManager.h"
#include "Grid/KalkiGridVisualizer.h"
#include "Grid/KalkiGridTypes.h"
#include "Logging/KalkiLog.h"
#include "Net/UnrealNetwork.h"

AKalkiLevelManager::AKalkiLevelManager()
{
    PrimaryActorTick.bCanEverTick = false;
    
    // Enable replication
    bReplicates = true;
    bAlwaysRelevant = true;    
    SetNetUpdateFrequency(1.0f);
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

    // ⭐ Bind to grid created event (everyone listens)
    GridManager->OnGridCreated.AddDynamic(this, &AKalkiLevelManager::OnGridCreated);

    // Only server creates grid (clients will build from replicated config)
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

void AKalkiLevelManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Unbind from events
    if (GridManager)
    {
        GridManager->OnGridCreated.RemoveDynamic(this, &AKalkiLevelManager::OnGridCreated);
    }

    // Clean up visualizer
    DestroyGridVisualizer();

    Super::EndPlay(EndPlayReason);
}

void AKalkiLevelManager::InitializeLevel()
{
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("InitializeLevel called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    KalkiLog::System(TEXT("Initializing level..."), EKalkiLogSeverity::Log, this);

    // Create grid (will trigger OnGridCreated event)
    CreateGrid();

    // Future: Spawn characters, set up encounters, etc.
}

void AKalkiLevelManager::CreateGrid()
{
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

    // This will trigger OnGridCreated event when complete
    GridManager->CreateGrid(GridSizeX, GridSizeY, TileSize, GridOrigin);

    KalkiLog::System(
        FString::Printf(TEXT("Grid created: %dx%d tiles [Server]"), GridSizeX, GridSizeY),
        EKalkiLogSeverity::Log,
        this
    );
}

void AKalkiLevelManager::OnGridCreated()
{
    // ⭐ Event-based: Grid is ready, spawn visualizer
    KalkiLog::System(
        FString::Printf(TEXT("OnGridCreated event received [Role: %s]"), 
            HasAuthority() ? TEXT("Server") : TEXT("Client")),
        EKalkiLogSeverity::Log,
        this
    );

    if (bAutoSpawnVisualizer && GridVisualizerClass)
    {
        SpawnGridVisualizer();
    }
}

void AKalkiLevelManager::SpawnGridVisualizer()
{
    if (!GridVisualizerClass)
    {
        KalkiLog::System(TEXT("LevelManager - GridVisualizerClass not set"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (GridVisualizer)
    {
        KalkiLog::System(TEXT("LevelManager - GridVisualizer already exists"), EKalkiLogSeverity::Warning, this);
        return;
    }

    // Spawn visualizer at grid origin
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GridVisualizer = GetWorld()->SpawnActor<AKalkiGridVisualizer>(
        GridVisualizerClass,
        GridOrigin,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (GridVisualizer)
    {
        KalkiLog::System(TEXT("LevelManager - GridVisualizer spawned"), EKalkiLogSeverity::Log, this);

        // Set initial visibility based on settings
        // Default to hidden, will show when entering combat mode
        GridVisualizer->HideGrid();
    }
    else
    {
        KalkiLog::System(TEXT("LevelManager - Failed to spawn GridVisualizer"), EKalkiLogSeverity::Error, this);
    }
}

void AKalkiLevelManager::DestroyGridVisualizer()
{
    if (GridVisualizer)
    {
        GridVisualizer->Destroy();
        GridVisualizer = nullptr;
        
        KalkiLog::System(TEXT("LevelManager - GridVisualizer destroyed"), EKalkiLogSeverity::Log, this);
    }
}

void AKalkiLevelManager::ShowGridVisualizer()
{
    if (GridVisualizer)
    {
        GridVisualizer->ShowGrid();
    }
}

void AKalkiLevelManager::HideGridVisualizer()
{
    if (GridVisualizer)
    {
        GridVisualizer->HideGrid();
    }
}

void AKalkiLevelManager::SetTileElevation(int32 X, int32 Y, float Elevation)
{
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
    if (!HasAuthority())
    {
        KalkiLog::System(TEXT("CreateRamp called on client - ignoring"), EKalkiLogSeverity::Warning, this);
        return;
    }

    if (!GridManager)
    {
        return;
    }

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