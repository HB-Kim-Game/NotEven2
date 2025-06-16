// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyUI.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "GameManager/NotEvenGameInstance.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UNotEvenGameInstance>(GetGameInstance());

	GameInstance->OnBeginSearch.AddDynamic(this, &ULobbyUI::ULobbyUI::SearchStart);
	GameInstance->OnSearchComplete.AddDynamic(this, &ULobbyUI::ULobbyUI::SearchComplete);

	StartButton->OnClicked.AddDynamic(this, &ULobbyUI::StartGame);
	FindButton->OnClicked.AddDynamic(this, &ULobbyUI::FindGame);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void ULobbyUI::StartGame()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(false);
	LoadingPanel->SetVisibility(ESlateVisibility::Visible);
	
	if (GetWorld()->GetFirstPlayerController()->HasAuthority())
	{
		GetWorld()->ServerTravel("/Game/KHB/Maps/MapDesign?listen?port=7777");
	}
}

void ULobbyUI::FindGame()
{
	if (GameInstance)
	{
		GameInstance->FindOtherSessions();
	}
}

void ULobbyUI::ExitGame()
{
	if (GameInstance)
	{
		GameInstance->DestroySession();
	}
}

void ULobbyUI::SetButtonEnabled(bool enabled)
{
	StartButton->SetIsEnabled(enabled);
	FindButton->SetIsEnabled(enabled);
	ExitButton->SetIsEnabled(enabled);
}

void ULobbyUI::SearchStart()
{
	LoadingPanel->SetVisibility(ESlateVisibility::Visible);
}

void ULobbyUI::SearchComplete()
{
	LoadingPanel->SetVisibility(ESlateVisibility::Hidden);
}
