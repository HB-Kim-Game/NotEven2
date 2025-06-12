// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"
#include "PlayTimeUI.h"
#include "Components/Image.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerUI::GameStart()
{
	if (!Dynamic_Mask)
	{
		Dynamic_Mask = UMaterialInstanceDynamic::Create(MaskMaterial, nullptr);
		//float aspect = Mask->GetDesiredSize().Y / Mask->GetDesiredSize().X; 
		//Dynamic_Mask->SetScalarParameterValue(FName("AspectScale"), aspect);
		Mask->SetBrushFromMaterial(Dynamic_Mask);
	}
	
	GetWorld()->GetTimerManager().SetTimer(StartHandle, FTimerDelegate::CreateLambda([&]()
	{
		float maskRadius;
		Mask->GetDynamicMaterial()->GetScalarParameterValue(FMaterialParameterInfo(FName("Radius")), maskRadius);
		maskRadius+= 0.05f / 1.5f;
		Mask->GetDynamicMaterial()->SetScalarParameterValue(FName("Radius"), maskRadius);

		if (maskRadius > 0.75f && !IsPlayingAnimation())
		{
			PlayAnimation(Start);
		}
		if (maskRadius > 1.f)
		{
			GetWorld()->GetTimerManager().ClearTimer(StartHandle);
		}
	}), 0.05f, true);
}
