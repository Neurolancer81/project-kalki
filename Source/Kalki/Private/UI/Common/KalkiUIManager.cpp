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

// In KalkiUIManager.cpp

void UKalkiUIManager::InitializeViewModels()
{
    FString LogPathTest = FPaths::ProjectSavedDir() / TEXT("crash_debug.txt");
    
    FFileHelper::SaveStringToFile(
        TEXT("InitializeViewModels START\n"), 
        *LogPathTest, 
        FFileHelper::EEncodingOptions::AutoDetect, 
        &IFileManager::Get(), 
        FILEWRITE_Append
    );

    FFileHelper::SaveStringToFile(
        FString::Printf(TEXT("InitializeViewModels - Registered classes: %d\n"), RegisteredViewModelClasses.Num()), 
        *LogPathTest, 
        FFileHelper::EEncodingOptions::AutoDetect, 
        &IFileManager::Get(), 
        FILEWRITE_Append
    );

    for (int32 i = 0; i < RegisteredViewModelClasses.Num(); ++i)
    {
        TSubclassOf<UKalkiViewModelBase> VMClass = RegisteredViewModelClasses[i];
        
        FFileHelper::SaveStringToFile(
            FString::Printf(TEXT("InitializeViewModels - Processing index %d\n"), i), 
            *LogPathTest, 
            FFileHelper::EEncodingOptions::AutoDetect, 
            &IFileManager::Get(), 
            FILEWRITE_Append
        );

        if (!VMClass)
        {
            FFileHelper::SaveStringToFile(
                TEXT("InitializeViewModels - VMClass is null, skipping\n"), 
                *LogPathTest, 
                FFileHelper::EEncodingOptions::AutoDetect, 
                &IFileManager::Get(), 
                FILEWRITE_Append
            );
            continue;
        }

        FFileHelper::SaveStringToFile(
            FString::Printf(TEXT("InitializeViewModels - VMClass name: %s\n"), *VMClass->GetName()), 
            *LogPathTest, 
            FFileHelper::EEncodingOptions::AutoDetect, 
            &IFileManager::Get(), 
            FILEWRITE_Append
        );

        // Check if already created
        if (ViewModels.Contains(VMClass))
        {
            FFileHelper::SaveStringToFile(
                TEXT("InitializeViewModels - Already exists, skipping\n"), 
                *LogPathTest, 
                FFileHelper::EEncodingOptions::AutoDetect, 
                &IFileManager::Get(), 
                FILEWRITE_Append
            );
            continue;
        }

        FFileHelper::SaveStringToFile(
            TEXT("InitializeViewModels - About to call NewObject\n"), 
            *LogPathTest, 
            FFileHelper::EEncodingOptions::AutoDetect, 
            &IFileManager::Get(), 
            FILEWRITE_Append
        );

        // Create ViewModel
        UKalkiViewModelBase* NewVM = NewObject<UKalkiViewModelBase>(this, VMClass);
        
        FFileHelper::SaveStringToFile(
            TEXT("InitializeViewModels - NewObject returned\n"), 
            *LogPathTest, 
            FFileHelper::EEncodingOptions::AutoDetect, 
            &IFileManager::Get(), 
            FILEWRITE_Append
        );

        if (NewVM)
        {
            FFileHelper::SaveStringToFile(
                TEXT("InitializeViewModels - Adding to ViewModels map\n"), 
                *LogPathTest, 
                FFileHelper::EEncodingOptions::AutoDetect, 
                &IFileManager::Get(), 
                FILEWRITE_Append
            );
            
            ViewModels.Add(VMClass, NewVM);
            
            FFileHelper::SaveStringToFile(
                TEXT("InitializeViewModels - About to call OnInitialize\n"), 
                *LogPathTest, 
                FFileHelper::EEncodingOptions::AutoDetect, 
                &IFileManager::Get(), 
                FILEWRITE_Append
            );
            
            NewVM->OnInitialize();
            
            FFileHelper::SaveStringToFile(
                TEXT("InitializeViewModels - OnInitialize completed\n"), 
                *LogPathTest, 
                FFileHelper::EEncodingOptions::AutoDetect, 
                &IFileManager::Get(), 
                FILEWRITE_Append
            );
        }
        else
        {
            FFileHelper::SaveStringToFile(
                TEXT("InitializeViewModels - NewObject returned null\n"), 
                *LogPathTest, 
                FFileHelper::EEncodingOptions::AutoDetect, 
                &IFileManager::Get(), 
                FILEWRITE_Append
            );
        }
    }

    FFileHelper::SaveStringToFile(
        TEXT("InitializeViewModels END\n"), 
        *LogPathTest, 
        FFileHelper::EEncodingOptions::AutoDetect, 
        &IFileManager::Get(), 
        FILEWRITE_Append
    );
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
