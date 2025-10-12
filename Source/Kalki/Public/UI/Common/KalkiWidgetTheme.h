// Copyright of V.S. Puranam and no one else

// Public/UI/KalkiWidgetTheme.h

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Styling/SlateBrush.h"
#include "KalkiWidgetTheme.generated.h"

/**
 * Theme asset for Kalki widgets
 * Defines colors, fonts, and styling for consistent UI appearance
 * Create instances as DataAssets in the editor
 */
UCLASS(BlueprintType)
class KALKI_API UKalkiWidgetTheme : public UDataAsset
{
    GENERATED_BODY()

public:
    // Primary colors
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor PrimaryColor = FLinearColor(0.1f, 0.1f, 0.1f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor SecondaryColor = FLinearColor(0.2f, 0.2f, 0.2f, 0.9f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor AccentColor = FLinearColor(0.8f, 0.6f, 0.2f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor HighlightColor = FLinearColor(1.0f, 0.8f, 0.3f, 1.0f);

    // Text colors
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor TextColor = FLinearColor::White;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor TextColorSecondary = FLinearColor(0.7f, 0.7f, 0.7f, 1.0f);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors")
    FLinearColor TextColorDisabled = FLinearColor(0.4f, 0.4f, 0.4f, 1.0f);

    // Semantic colors
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors|Semantic")
    FLinearColor SuccessColor = FLinearColor::Green;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors|Semantic")
    FLinearColor WarningColor = FLinearColor::Yellow;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors|Semantic")
    FLinearColor ErrorColor = FLinearColor::Red;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Colors|Semantic")
    FLinearColor InfoColor = FLinearColor(0.3f, 0.6f, 1.0f, 1.0f);

    // Fonts
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Typography")
    FSlateFontInfo PrimaryFont;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Typography")
    FSlateFontInfo HeaderFont;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Typography")
    FSlateFontInfo SmallFont;

    // Spacing
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Layout")
    float PaddingSmall = 4.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Layout")
    float PaddingMedium = 8.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Layout")
    float PaddingLarge = 16.0f;

    // Border/Background
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Style")
    FSlateBrush BackgroundBrush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Style")
    FSlateBrush BorderBrush;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Theme|Style")
    float BorderRadius = 4.0f;

    UKalkiWidgetTheme()
    {
        // Set default fonts
        PrimaryFont.Size = 14;
        HeaderFont.Size = 18;
        SmallFont.Size = 10;
    }
};