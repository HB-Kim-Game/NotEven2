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

	virtual void NativeConstruct() override;

	UFUNCTION()
	void StartGame();
};
