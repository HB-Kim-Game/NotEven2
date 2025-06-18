// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyUI.h"

#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "GameManager/NotEvenGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/LobbyGameMode.h"

void ULobbyUI::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<UNotEvenGameInstance>(GetGameInstance());

	GameInstance->OnBeginSearch.AddDynamic(this, &ULobbyUI::ULobbyUI::SearchStart);
	GameInstance->OnSearchComplete.AddDynamic(this, &ULobbyUI::ULobbyUI::SearchComplete);

	StartButton->OnClicked.AddDynamic(this, &ULobbyUI::StartGame);
	FindButton->OnClicked.AddDynamic(this, &ULobbyUI::FindGame);
	ExitButton->OnClicked.AddDynamic(this, &ULobbyUI::ExitGame);

	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	GetWorld()->GetFirstPlayerController()->DefaultMouseCursor = EMouseCursor::Type::Default;
}

void ULobbyUI::StartGame()
{
	LoadingPanel->SetVisibility(ESlateVisibility::Visible);
	UGameplayStatics::PlaySound2D(GetWorld(), ConfirmSound);
	
	if (GetWorld()->GetFirstPlayerController()->HasAuthority())
	{
		Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->ShowLoadingScreen();
		GetWorld()->ServerTravel("/Game/KHB/Maps/MapDesign?listen?port=7777");
	}
}

void ULobbyUI::FindGame()
{
	if (GameInstance)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ConfirmSound);
		GameInstance->FindOtherSessions();
	}
}

void ULobbyUI::ExitGame()
{
	if (GameInstance)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ConfirmSound);
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
