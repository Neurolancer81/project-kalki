// Private/Logging/KalkiLogFileWriter.cpp

#include "Logging/KalkiLogFileWriter.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/Paths.h"

FKalkiLogFileWriter::FKalkiLogFileWriter(const FString& InFilePath)
    : FilePath(InFilePath)
    , FileHandle(nullptr)
    , Thread(nullptr)
    , bIsRunning(false)
    , bStopRequested(false)
{
    // Create the thread
    Thread = FRunnableThread::Create(this, TEXT("KalkiLogFileWriter"), 0, TPri_BelowNormal);
}

FKalkiLogFileWriter::~FKalkiLogFileWriter()
{
    // Stop the thread
    FKalkiLogFileWriter::Stop();

    if (Thread)
    {
        Thread->WaitForCompletion();
        delete Thread;
        Thread = nullptr;
    }

    // Close file
    if (FileHandle)
    {
        delete FileHandle;
        FileHandle = nullptr;
    }
}

bool FKalkiLogFileWriter::Init()
{
    // Open file for writing
    IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
    
    // Ensure directory exists
    FString Directory = FPaths::GetPath(FilePath);
    if (!PlatformFile.DirectoryExists(*Directory))
    {
        PlatformFile.CreateDirectoryTree(*Directory);
    }

    // Open file (append mode)
    FileHandle = PlatformFile.OpenWrite(*FilePath, true);
    
    if (!FileHandle)
    {
        UE_LOG(LogTemp, Error, TEXT("KalkiLogFileWriter: Failed to open log file: %s"), *FilePath);
        return false;
    }

    bIsRunning = true;
    return true;
}

uint32 FKalkiLogFileWriter::Run()
{
    while (!bStopRequested)
    {
        ProcessQueue();
        
        // Sleep briefly to avoid spinning
        FPlatformProcess::Sleep(0.1f);
    }

    // Final flush before exit
    ProcessQueue();

    return 0;
}

void FKalkiLogFileWriter::Stop()
{
    bStopRequested = true;
}

void FKalkiLogFileWriter::Exit()
{
    bIsRunning = false;
}

void FKalkiLogFileWriter::QueueMessage(const FString& Message)
{
    MessageQueue.Enqueue(Message);
}

void FKalkiLogFileWriter::Flush()
{
    ProcessQueue();
    
    if (FileHandle)
    {
        FileHandle->Flush();
    }
}

void FKalkiLogFileWriter::ProcessQueue()
{
    if (!FileHandle)
    {
        return;
    }

    FString Message;
    while (MessageQueue.Dequeue(Message))
    {
        // Convert to UTF8
        FTCHARToUTF8 UTF8String(*Message);
        
        // Write to file
        FileHandle->Write(reinterpret_cast<const uint8*>(UTF8String.Get()), UTF8String.Length());
    }
}