// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUI.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API ULobbyUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	class UButton* StartButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* FindButton;
	UPROPERTY(meta=(BindWidget))
	class UButton* ExitButton;

	UPROPERTY(meta=(BindWidget))
	class UCanvasPanel* LoadingPanel;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();

	UFUNCTION()
	void FindGame();

	UFUNCTION()
	void ExitGame();

	void SetButtonEnabled(bool enabled);

	UFUNCTION()
	void SearchStart();
	UFUNCTION()
	void SearchComplete();

	UPROPERTY()
	class UNotEvenGameInstance* GameInstance;
};
