// Copyright of V.S. Puranam and no one else


// Private/Grid/KalkiGridTile.cpp

#include "Grid/KalkiGridTile.h"

#include "Grid/KalkiGridManager.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"

AKalkiGridTile::AKalkiGridTile()
{
    PrimaryActorTick.bCanEverTick = false;

    // Create root component
    TileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("TileRoot"));
    RootComponent = TileRoot;

    // Create tile mesh (flat plane)
    TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    TileMesh->SetupAttachment(TileRoot);
    TileMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    TileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    TileMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // For mouse clicks

    // Mesh will be set in Blueprint or by GridManager
    // Default scale for 100x100 unit tile (if using default plane which is 200x200)
    TileMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.0f));

    // Start hidden - grid manager will show tiles as needed
    TileMesh->SetVisibility(false);
}

void AKalkiGridTile::SetTerrainType(const EKalkiTerrainType Type)
{
    TerrainType = Type;
    
    // Update properties based on terrain
    switch (Type)
    {
    case EKalkiTerrainType::Normal:
        SetMovementCost(1);
        SetWalkable(true);
        break;
            
    case EKalkiTerrainType::Difficult:
        SetMovementCost(2);
        SetWalkable(true);
        break;
            
    case EKalkiTerrainType::Wall:
        SetWalkable(false);
        break;
            
    case EKalkiTerrainType::Water:
        SetMovementCost(2);
        SetWalkable(true); // Maybe false for non-swimmers
        break;
            
    case EKalkiTerrainType::Hazard:
        SetMovementCost(1);
        SetWalkable(true); // Walkable but damages
        break;
            
    case EKalkiTerrainType::Cover:
        SetMovementCost(1);
        SetWalkable(true);
        break;
    }
}

void AKalkiGridTile::BeginPlay()
{
    Super::BeginPlay();

    // Create dynamic material instance
    if (TileMesh && TileMesh->GetMaterial(0))
    {
        DynamicMaterial = TileMesh->CreateDynamicMaterialInstance(0);
        UpdateMaterial();
    }

    // Register with grid manager
    RegisterWithGridManager();
}

void AKalkiGridTile::RegisterWithGridManager()
{
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }

    // Get the grid manager subsystem
    UKalkiGridManager* GridManager = World->GetSubsystem<UKalkiGridManager>();
    if (GridManager)
    {
        GridManager->RegisterTile(this);
    }
}

void AKalkiGridTile::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // Update material in editor
    if (TileMesh && TileMesh->GetMaterial(0))
    {
        UMaterialInstanceDynamic* PreviewMaterial = TileMesh->CreateDynamicMaterialInstance(0);
        if (PreviewMaterial)
        {
            PreviewMaterial->SetVectorParameterValue(TEXT("Color"), BaseColor);
        }
    }
}

void AKalkiGridTile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Unregister from grid manager
    UWorld* World = GetWorld();
    if (World)
    {
        UKalkiGridManager* GridManager = World->GetSubsystem<UKalkiGridManager>();
        if (GridManager)
        {
            GridManager->UnregisterTile(this);
        }
    }
    
    Super::EndPlay(EndPlayReason);

    
}

void AKalkiGridTile::SetGridPosition(const FIntPoint& Position)
{
    GridPosition = Position;

#if WITH_EDITORONLY_DATA
    // Update actor label in editor for easy identification
    SetActorLabel(FString::Printf(TEXT("GridTile_%d_%d"), Position.X, Position.Y));
#endif
}

void AKalkiGridTile::SetOccupyingCharacter(AKalkiCharacter* KalkiCharacter)
{
    OccupyingCharacter = KalkiCharacter;

    // TODO: Update visual state when occupied
}

void AKalkiGridTile::ClearOccupyingCharacter()
{
    OccupyingCharacter = nullptr;
}

void AKalkiGridTile::SetWalkable(bool bWalkable)
{
    bIsWalkable = bWalkable;

    // TODO: Update visual to show unwalkable tiles differently
}

void AKalkiGridTile::SetHighlighted(bool bHighlight, FLinearColor Color)
{
    bIsHighlighted = bHighlight;
    
    if (bHighlight)
    {
        HighlightColor = Color;
    }

    UpdateMaterial();
}

void AKalkiGridTile::UpdateMaterial()
{
    if (!DynamicMaterial)
    {
        return;
    }

    FLinearColor ColorToUse = bIsHighlighted ? HighlightColor : BaseColor;
    DynamicMaterial->SetVectorParameterValue(TEXT("Color"), ColorToUse);

    // Adjust opacity based on highlight state
    float Opacity = bIsHighlighted ? 0.8f : 0.3f;
    ColorToUse.A = Opacity;
    DynamicMaterial->SetVectorParameterValue(TEXT("Color"), ColorToUse);
}
