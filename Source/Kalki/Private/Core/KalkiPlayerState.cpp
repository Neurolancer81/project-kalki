// Private/Core/KalkiPlayerState.cpp

#include "Core/KalkiPlayerState.h"
#include "Net/UnrealNetwork.h"

AKalkiPlayerState::AKalkiPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, TeamResources(0)
{
	SetNetUpdateFrequency(100.f);
}

void AKalkiPlayerState::AddControlledCharacter(AKalkiCharacter* Character)
{
	if (Character && !ControlledCharacters.Contains(Character))
	{
		ControlledCharacters.Add(Character);
	}
}

void AKalkiPlayerState::RemoveControlledCharacter(AKalkiCharacter* Character)
{
	ControlledCharacters.Remove(Character);
}

void AKalkiPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AKalkiPlayerState, ControlledCharacters);
	DOREPLIFETIME(AKalkiPlayerState, TeamResources);
}