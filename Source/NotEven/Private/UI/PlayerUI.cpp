// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUI.h"

#include "IngredientStruct.h"
#include "OrderListViewer.h"
#include "PlayTimeUI.h"
#include "PriceUI.h"

void UPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, [this]()
	{
		TArray<FRecipeIngredientData> array;
		FRecipeIngredientData d1;
		d1.IngredientID = TEXT("Fish");
		d1.RequireStates.Add(EIngredientState::Sliced);
		array.Add(d1);
		
		this->OrderListViewer->CheckOrderSuccess(array);
		
	}, 15.f, true);
}
