// Public/Logging/KalkiLogFileWriter.h

#pragma once

#include "CoreMinimal.h"
#include "HAL/Runnable.h"
#include "HAL/RunnableThread.h"
#include "Containers/Queue.h"
#include "Logging/KalkiLogTypes.h"

/**
 * Async file writer for log messages
 * Runs on separate thread to avoid blocking game thread
 */
class KALKI_API FKalkiLogFileWriter : public FRunnable
{
public:
	FKalkiLogFileWriter(const FString& InFilePath);
	virtual ~FKalkiLogFileWriter();

	// FRunnable interface
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
	virtual void Exit() override;

	// Queue a message for writing
	void QueueMessage(const FString& Message);

	// Flush all pending messages immediately
	void Flush();

	// Check if writer is running
	bool IsRunning() const { return bIsRunning; }

private:
	// File path
	FString FilePath;

	// File handle
	IFileHandle* FileHandle;

	// Thread
	FRunnableThread* Thread;

	// Message queue (thread-safe)
	TQueue<FString, EQueueMode::Mpsc> MessageQueue;

	// Running state
	FThreadSafeBool bIsRunning;
	FThreadSafeBool bStopRequested;

	// Write messages from queue to file
	void ProcessQueue();
};