// Copyright of V.S. Puranam and no one else


// Private/UI/KalkiWidgetBase.cpp

#include "UI/Widgets/KalkiViewModelWidget.h"
#include "Logging/KalkiLog.h"
#include "UI/Common/KalkiViewModelBase.h"

void UKalkiViewModelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// If ViewModel was set before construction, call binding now
	if (ViewModel)
	{
		OnViewModelSet();
        
		KalkiLog::System(
			FString::Printf(TEXT("Widget %s constructed with ViewModel"), *GetName()),
			EKalkiLogSeverity::Log,
			this
		);
	}
	else
	{
		KalkiLog::System(
			FString::Printf(TEXT("Widget %s constructed without ViewModel"), *GetName()),
			EKalkiLogSeverity::Warning,
			this
		);
	}
}

void UKalkiViewModelWidget::NativeDestruct()
{
	// Clear ViewModel bindings
	if (ViewModel)
	{
		OnViewModelCleared();
		ViewModel = nullptr;
	}

	Super::NativeDestruct();
}

void UKalkiViewModelWidget::SetViewModel(UKalkiViewModelBase* InViewModel)
{
	// Clear old ViewModel
	if (ViewModel) 
	{
		OnViewModelCleared();
	}

	// Set new ViewModel
	ViewModel = InViewModel;

	if (ViewModel)
	{
		KalkiLog::System(
			FString::Printf(TEXT("Widget %s bound to ViewModel %s"), *GetName(), *ViewModel->GetName()),
			EKalkiLogSeverity::Log,
			this
		);

		// Bind to new ViewModel (if widget is constructed)
		if (IsConstructed())
		{
			OnViewModelSet();
		}
	}
}
