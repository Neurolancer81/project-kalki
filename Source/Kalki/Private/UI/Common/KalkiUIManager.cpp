// Copyright of V.S. Puranam and no one else


// Private/UI/KalkiUIManager.cpp

#include "UI/Common/KalkiUIManager.h"
#include "UI/Common/KalkiViewModelBase.h"
#include "UI/Widgets/KalkiViewModelWidget.h"
#include "Logging/KalkiLog.h"
#include "UI/Common/KalkiWidgetTheme.h"

void UKalkiUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    InitializeViewModels();

    KalkiLog::System(
        FString::Printf(TEXT("UIManager initialized with %d ViewModels"), ViewModels.Num()),
        EKalkiLogSeverity::Log,
        this
    );
}

void UKalkiUIManager::Deinitialize()
{
    ShutdownViewModels();

    ViewModels.Empty();
    RegisteredViewModelClasses.Empty();

    KalkiLog::System(TEXT("UIManager deinitialized"), EKalkiLogSeverity::Log, this);

    Super::Deinitialize();
}

void UKalkiUIManager::RegisterViewModelClass(TSubclassOf<UKalkiViewModelBase> ViewModelClass)
{
    if (!ViewModelClass)
    {
        KalkiLog::System(
            TEXT("RegisterViewModelClass - Invalid class"),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    if (RegisteredViewModelClasses.Contains(ViewModelClass))
    {
        KalkiLog::System(
            FString::Printf(TEXT("RegisterViewModelClass - %s already registered"), *ViewModelClass->GetName()),
            EKalkiLogSeverity::Warning,
            this
        );
        return;
    }

    RegisteredViewModelClasses.Add(ViewModelClass);
    
    KalkiLog::System(
        FString::Printf(TEXT("Registered ViewModel class: %s"), *ViewModelClass->GetName()),
        EKalkiLogSeverity::Log,
        this
    );
}

// In KalkiUIManager.cpp - Update InitializeViewModels:

void UKalkiUIManager::InitializeViewModels()
{
    KalkiLog::System(
        FString::Printf(TEXT("InitializeViewModels - Creating %d ViewModels"), 
            RegisteredViewModelClasses.Num()),
        EKalkiLogSeverity::Log,
        this
    );

    for (TSubclassOf<UKalkiViewModelBase> VMClass : RegisteredViewModelClasses)
    {
        if (!VMClass)
        {
            continue;
        }

        // Check if already created
        if (ViewModels.Contains(VMClass))
        {
            KalkiLog::System(
                FString::Printf(TEXT("InitializeViewModels - %s already exists"), 
                    *VMClass->GetName()),
                EKalkiLogSeverity::Warning,
                this
            );
            continue;
        }

        // Create ViewModel
        UKalkiViewModelBase* NewVM = NewObject<UKalkiViewModelBase>(this, VMClass);
        if (NewVM)
        {
            ViewModels.Add(VMClass, NewVM);
            NewVM->OnInitialize();
            
            KalkiLog::System(
                FString::Printf(TEXT("InitializeViewModels - Created %s"), 
                    *VMClass->GetName()),
                EKalkiLogSeverity::Log,
                this
            );
        }
        else
        {
            KalkiLog::System(
                FString::Printf(TEXT("InitializeViewModels - Failed to create %s"), 
                    *VMClass->GetName()),
                EKalkiLogSeverity::Error,
                this
            );
        }
    }
}

void UKalkiUIManager::ShutdownViewModels()
{
    for (auto& Pair : ViewModels)
    {
        if (Pair.Value)
        {
            Pair.Value->Shutdown();
        }
    }
}

UKalkiViewModelWidget* UKalkiUIManager::CreateWidgetWithViewModelByClass(
    TSubclassOf<UKalkiViewModelWidget> WidgetClass,
    TSubclassOf<UKalkiViewModelBase> ViewModelClass)
{
    if (!WidgetClass || !ViewModelClass)
    {
        KalkiLog::System(
            TEXT("CreateWidgetWithViewModelByClass - Invalid class"),
            EKalkiLogSeverity::Warning,
            this
        );
        return nullptr;
    }

    UKalkiViewModelWidget* Widget = CreateWidget<UKalkiViewModelWidget>(GetWorld(), WidgetClass);
    if (Widget)
    {
        if (TObjectPtr<UKalkiViewModelBase> const* FoundVM = ViewModels.Find(ViewModelClass))
        {
            Widget->SetViewModel(*FoundVM);
        }
        else
        {
            KalkiLog::System(
                FString::Printf(TEXT("CreateWidgetWithViewModelByClass - ViewModel %s not found"), 
                    *ViewModelClass->GetName()),
                EKalkiLogSeverity::Warning,
                this
            );
        }
    }

    return Widget;
}

void UKalkiUIManager::SetGlobalTheme(UKalkiWidgetTheme* InTheme)
{
    GlobalTheme = InTheme;
    
    KalkiLog::System(
        FString::Printf(TEXT("Global theme set: %s"), InTheme ? *InTheme->GetName() : TEXT("None")),
        EKalkiLogSeverity::Log,
        this
    );
}
