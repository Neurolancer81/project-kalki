// Copyright of V.S. Puranam and no one else

// Private/UI/CombatLog/KalkiLogMessageWidget.cpp
// REPLACE with this updated version

#include "UI/CombatLog/KalkiLogMessageWidget.h"
#include "UI/Common/KalkiWidgetTheme.h"
#include "Components/TextBlock.h"

void UKalkiLogMessageWidget::SetMessage(const FKalkiLogDisplayEntry& InEntry)
{
	MessageEntry = InEntry;

	if (MessageText)
	{
		MessageText->SetText(MessageEntry.DisplayText);
        
		// Use themed severity color
		FLinearColor Color = GetSeverityColor(MessageEntry.Severity);
		MessageText->SetColorAndOpacity(FSlateColor(Color));
	}
	else
	{
		
	}
}

void UKalkiLogMessageWidget::OnThemeApplied()
{
	Super::OnThemeApplied();

	// Reapply message styling with new theme
	if (MessageText && MessageEntry.DisplayText.IsEmpty() == false)
	{
		FLinearColor Color = GetSeverityColor(MessageEntry.Severity);
		MessageText->SetColorAndOpacity(FSlateColor(Color));
	}
}

void UKalkiLogMessageWidget::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	// TODO: Future implementation - show tooltip with metadata
	// For now, brighten the color slightly on hover
	if (MessageText)
	{
		FLinearColor BaseColor = GetSeverityColor(MessageEntry.Severity);
		FLinearColor HighlightColor = BaseColor * 1.3f;
		HighlightColor.A = BaseColor.A; // Keep original alpha
		MessageText->SetColorAndOpacity(FSlateColor(HighlightColor));
	}
}

void UKalkiLogMessageWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	// Reset to themed color
	if (MessageText)
	{
		FLinearColor Color = GetSeverityColor(MessageEntry.Severity);
		MessageText->SetColorAndOpacity(FSlateColor(Color));
	}
}