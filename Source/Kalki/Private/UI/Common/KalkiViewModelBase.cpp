// Copyright of V.S. Puranam and no one else


// Private/UI/KalkiViewModelBase.cpp


#include "UI/Common/KalkiViewModelBase.h"

#include "Logging/KalkiLog.h"

UKalkiViewModelBase::UKalkiViewModelBase()
	: bInitialized(false)
	, bEnabled(true)
{
}

void UKalkiViewModelBase::Initialize(UWorld* InWorld)
{
	if (bInitialized)
	{
		KalkiLog::System(
			FString::Printf(TEXT("ViewModel %s already initialized"), *GetName()),
			EKalkiLogSeverity::Warning,
			this
		);
		return;
	}

	if (!InWorld)
	{
		KalkiLog::System(
			FString::Printf(TEXT("ViewModel %s - Cannot initialize with null world"), *GetName()),
			EKalkiLogSeverity::Error,
			this
		);
		return;
	}

	WorldContext = InWorld;
	bInitialized = true;

	// Call virtual function for subclass initialization
	OnInitialize();

	KalkiLog::System(
		FString::Printf(TEXT("ViewModel %s initialized"), *GetName()),
		EKalkiLogSeverity::Log,
		this
	);
}

void UKalkiViewModelBase::Shutdown()
{
	if (!bInitialized)
	{
		return;
	}

	// Call virtual function for subclass cleanup
	OnShutdown();

	KalkiLog::System(
		FString::Printf(TEXT("ViewModel %s shut down"), *GetName()),
		EKalkiLogSeverity::Log,
		this
	);

	WorldContext = nullptr;
	bInitialized = false;
}

void UKalkiViewModelBase::SetEnabled(bool bInEnabled)
{
	if (bEnabled == bInEnabled)
	{
		return;
	}

	bEnabled = bInEnabled;
	OnEnabledChanged(bEnabled);

	KalkiLog::System(
		FString::Printf(TEXT("ViewModel %s %s"), *GetName(), bEnabled ? TEXT("enabled") : TEXT("disabled")),
		EKalkiLogSeverity::Log,
		this
	);
}

UWorld* UKalkiViewModelBase::GetWorld() const
{
	// ViewModels are owned by UIManager (a WorldSubsystem)
	// Get World from our Outer
	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
    
	return nullptr;
}

void UKalkiViewModelBase::OnInitialize()
{
	if (bInitialized)
	{
		KalkiLog::System(
			FString::Printf(TEXT("ViewModel %s already initialized"), *GetName()),
			EKalkiLogSeverity::Warning,
			this
		);
		return;
	}

	bInitialized = true;

	KalkiLog::System(
		FString::Printf(TEXT("ViewModel %s initialized"), *GetClass()->GetName()),
		EKalkiLogSeverity::Log,
		this
	);
}

void UKalkiViewModelBase::OnShutdown()
{
	if (!bInitialized)
	{
		return;
	}

	bInitialized = false;

	KalkiLog::System(
		FString::Printf(TEXT("ViewModel %s shutdown"), *GetClass()->GetName()),
		EKalkiLogSeverity::Log,
		this
	);
}
