// Copyright of V.S. Puranam and no one else

// Private/Grid/KalkiGridLibrary.cpp

#include "Grid/KalkiGridLibrary.h"

int32 UKalkiGridLibrary::GetManhattanDistance(const FIntPoint& A, const FIntPoint& B)
{
    return FMath::Abs(A.X - B.X) + FMath::Abs(A.Y - B.Y);
}

float UKalkiGridLibrary::GetEuclideanDistance(const FIntPoint& A, const FIntPoint& B)
{
    int32 DX = A.X - B.X;
    int32 DY = A.Y - B.Y;
    return FMath::Sqrt(static_cast<float>(DX * DX + DY * DY));
}

int32 UKalkiGridLibrary::GetChebyshevDistance(const FIntPoint& A, const FIntPoint& B)
{
    return FMath::Max(FMath::Abs(A.X - B.X), FMath::Abs(A.Y - B.Y));
}

TArray<FIntPoint> UKalkiGridLibrary::GetOrthogonalNeighbors(const FIntPoint& Position)
{
    TArray<FIntPoint> Neighbors;
    Neighbors.Reserve(4);

    // North, South, East, West
    Neighbors.Add(FIntPoint(Position.X, Position.Y + 1)); // North
    Neighbors.Add(FIntPoint(Position.X, Position.Y - 1)); // South
    Neighbors.Add(FIntPoint(Position.X + 1, Position.Y)); // East
    Neighbors.Add(FIntPoint(Position.X - 1, Position.Y)); // West

    return Neighbors;
}

TArray<FIntPoint> UKalkiGridLibrary::GetAllNeighbors(const FIntPoint& Position)
{
    TArray<FIntPoint> Neighbors;
    Neighbors.Reserve(8);

    // All 8 directions
    for (int32 DX = -1; DX <= 1; ++DX)
    {
        for (int32 DY = -1; DY <= 1; ++DY)
        {
            // Skip center (0, 0)
            if (DX == 0 && DY == 0)
            {
                continue;
            }

            Neighbors.Add(FIntPoint(Position.X + DX, Position.Y + DY));
        }
    }

    return Neighbors;
}

float UKalkiGridLibrary::GetMovementCost(const FIntPoint& From, const FIntPoint& To)
{
    int32 DX = FMath::Abs(To.X - From.X);
    int32 DY = FMath::Abs(To.Y - From.Y);

    // Both X and Y changed = diagonal movement
    if (DX > 0 && DY > 0)
    {
        return DIAGONAL_MOVE_COST;
    }

    // Only X or Y changed = orthogonal movement
    return ORTHOGONAL_MOVE_COST;
}

bool UKalkiGridLibrary::AreAdjacent(const FIntPoint& A, const FIntPoint& B)
{
    int32 DX = FMath::Abs(A.X - B.X);
    int32 DY = FMath::Abs(A.Y - B.Y);

    // Adjacent if difference is at most 1 in each direction
    // and not the same position
    return (DX <= 1 && DY <= 1) && !(DX == 0 && DY == 0);
}

FIntPoint UKalkiGridLibrary::GetDirectionVector(const FIntPoint& From, const FIntPoint& To)
{
    int32 DX = To.X - From.X;
    int32 DY = To.Y - From.Y;

    // Normalize to -1, 0, or 1
    int32 NormalizedX = (DX > 0) ? 1 : (DX < 0) ? -1 : 0;
    int32 NormalizedY = (DY > 0) ? 1 : (DY < 0) ? -1 : 0;

    return FIntPoint(NormalizedX, NormalizedY);
}