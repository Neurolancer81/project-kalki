// Copyright of V.S. Puranam and no one else

// Private/UI/KalkiWidgetThemedBase.cpp


#include "UI/Widgets/KalkiWidgetBase.h"

#include "UI/Common/KalkiWidgetTheme.h"
#include "UI/Common/KalkiUIManager.h"
#include "Logging/KalkiLog.h"
#include "Animation/UMGSequencePlayer.h"
#include "Animation/WidgetAnimation.h"

void UKalkiWidgetBase::NativePreConstruct()
{
    Super::NativePreConstruct();

    // Apply theme in editor preview
    if (WidgetTheme)
    {
        ApplyTheme();
    }
}

void UKalkiWidgetBase::NativeConstruct()
{
    Super::NativeConstruct();

    // Get global theme if needed
    if (bUseGlobalTheme && !WidgetTheme)
    {
        if (UKalkiUIManager* UIManager = GetWorld()->GetSubsystem<UKalkiUIManager>())
        {
            WidgetTheme = UIManager->GetGlobalTheme();
        }
    }

    // Apply theme
    if (WidgetTheme)
    {
        ApplyTheme();
    }
    else
    {
        KalkiLog::System(
            FString::Printf(TEXT("Widget %s has no theme set"), *GetName()),
            EKalkiLogSeverity::Warning,
            this
        );
    }
}

void UKalkiWidgetBase::SetTheme(UKalkiWidgetTheme* InTheme)
{
    if (WidgetTheme == InTheme)
    {
        return;
    }

    WidgetTheme = InTheme;
    ApplyTheme();
}

void UKalkiWidgetBase::ApplyTheme()
{
    if (!WidgetTheme)
    {
        return;
    }

    // Cache colors from theme
    PrimaryColor = WidgetTheme->PrimaryColor;
    SecondaryColor = WidgetTheme->SecondaryColor;
    AccentColor = WidgetTheme->AccentColor;

    // Call virtual function for subclass-specific theming
    OnThemeApplied();
}

void UKalkiWidgetBase::SetPrimaryColor(FLinearColor Color)
{
    PrimaryColor = Color;
    OnThemeApplied();
}

void UKalkiWidgetBase::SetSecondaryColor(FLinearColor Color)
{
    SecondaryColor = Color;
    OnThemeApplied();
}

FLinearColor UKalkiWidgetBase::GetSeverityColor(EKalkiLogSeverity Severity) const
{
    if (!WidgetTheme)
    {
        // Fallback colors
        switch (Severity)
        {
            case EKalkiLogSeverity::Verbose:
                return FLinearColor::Gray;
            case EKalkiLogSeverity::Log:
                return FLinearColor::White;
            case EKalkiLogSeverity::Warning:
                return FLinearColor::Yellow;
            case EKalkiLogSeverity::Error:
                return FLinearColor::Red;
            default:
                return FLinearColor::White;
        }
    }

    // Use theme colors
    switch (Severity)
    {
        case EKalkiLogSeverity::Verbose:
            return WidgetTheme->TextColorSecondary;
        case EKalkiLogSeverity::Log:
            return WidgetTheme->InfoColor;
        case EKalkiLogSeverity::Warning:
            return WidgetTheme->WarningColor;
        case EKalkiLogSeverity::Error:
            return WidgetTheme->ErrorColor;
        default:
            return WidgetTheme->TextColor;
    }
}

void UKalkiWidgetBase::PlayShowAnimation()
{
    // Look for animation named "ShowAnim" in the widget
    if (UWidgetAnimation* ShowAnim = GetAnimationByName(FName("ShowAnim")))
    {
        PlayAnimation(ShowAnim);
    }
}

void UKalkiWidgetBase::PlayHideAnimation()
{
    // Look for animation named "HideAnim" in the widget
    if (UWidgetAnimation* HideAnim = GetAnimationByName(FName("HideAnim")))
    {
        PlayAnimation(HideAnim);
    }
}

UWidgetAnimation* UKalkiWidgetBase::GetAnimationByName(FName AnimName) const
{
    // Search through widget animations
    TArray<UWidgetAnimation*> Animations;
    
    #if WITH_EDITOR
    // In editor, use reflection to find animations
    for (TFieldIterator<FObjectProperty> It(GetClass()); It; ++It)
    {
        if (It->PropertyClass == UWidgetAnimation::StaticClass())
        {
            if (UWidgetAnimation* Anim = Cast<UWidgetAnimation>(It->GetPropertyValue_InContainer(this)))
            {
                if (Anim->GetFName() == AnimName)
                {
                    return Anim;
                }
            }
        }
    }
    #endif

    return nullptr;
}