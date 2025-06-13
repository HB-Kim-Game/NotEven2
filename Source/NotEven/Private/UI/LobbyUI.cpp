// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyUI.h"

#include "Components/Button.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &ULobbyUI::StartGame);
}

void ULobbyUI::StartGame()
{
	if (GetWorld()->GetFirstPlayerController()->HasAuthority())
	{
		GetWorld()->ServerTravel("/Game/KHB/Maps/MapDesign?listen", true);
	}
}
