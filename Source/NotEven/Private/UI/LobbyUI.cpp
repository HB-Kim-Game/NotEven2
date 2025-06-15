// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyUI.h"

#include "Components/Button.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	StartButton->OnClicked.AddDynamic(this, &ULobbyUI::StartGame);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void ULobbyUI::StartGame()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	
	if (GetWorld()->GetFirstPlayerController()->HasAuthority())
	{
		GetWorld()->ServerTravel("/Game/KHB/Maps/MapDesign?listen", true);
	}
}
