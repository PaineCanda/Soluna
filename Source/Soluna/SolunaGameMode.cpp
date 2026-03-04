// Copyright Epic Games, Inc. All Rights Reserved.

#include "SolunaGameMode.h"

ASolunaGameMode::ASolunaGameMode()
{
	// stub
}

void ASolunaGameMode::HostLANGame()
{
	GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/Lvl_ThirdPerson?Listen");
}

void ASolunaGameMode::JoinLANGame()
{
	APlayerController *PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (PC)
	{
		PC->ClientTravel("192.168.4.203", ETravelType::TRAVEL_Absolute);
	}
}
