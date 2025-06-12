// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CookingIcon.h"

#include "Animation/UMGSequencePlayer.h"
#include "Components/Image.h"

void UCookingIcon::ShowIcon(EIngredientState state)
{
	StopAllAnimations();
	if (auto v = Icons.Find(state))
	{
		SetColorAndOpacity(FLinearColor(1,1,1,0));
		Border->SetColorAndOpacity(FLinearColor::White);
		BackgroundImage->SetColorAndOpacity(v->BackgroundColor);
		IconImage->SetBrushFromTexture(v->IconImage);

		if (state == EIngredientState::Burned) SetColorAndOpacity(FLinearColor::White);
		else PlayAnimation(Appear);
	}
}

void UCookingIcon::ShowWarning(float Speed)
{
	Border->SetColorAndOpacity(FLinearColor::Black);
	BackgroundImage->SetColorAndOpacity(WarningColor);
	IconImage->SetBrushFromTexture(WarningIcon);
	if (WarningPlayer && WarningPlayer->IsPlayingForward())
	{
		WarningPlayer->SetPlaybackSpeed(Speed);
		return;
	}
	WarningPlayer = PlayAnimation(Warning, 0, 0, EUMGSequencePlayMode::PingPong, Speed);
}
