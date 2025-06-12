// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SubmitFoodUI.h"
#include "IngredientStruct.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"

void USubmitFoodUI::ShowIconImage(TArray<FRecipeIngredientData> data)
{
	if (!IconImages.IsEmpty())
	{
		for (auto i : IconImages)
		{
			if (i == nullptr) continue;
			i->SetVisibility(ESlateVisibility::Collapsed);
		}	
	}
	
	for (int32 i = 0; i < data.Num(); i++)
	{
		if (i > IconImages.Num() - 1)
		{
			if(UImage* iconImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass()))
			{
				auto* mat = UMaterialInstanceDynamic::Create(IconMaterialInstance, this);
				iconImage->SetBrushFromMaterial(mat);
				if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *data[i].IconImagePath))
				{
					mat->SetTextureParameterValue(FName("IconImage"), texture);
				}
				UUniformGridSlot* slot = Parent->AddChildToUniformGrid(iconImage, i / 2, i % 2);
				iconImage->SetDesiredSizeOverride(FVector2D(32.f, 32.f));
				slot->SetHorizontalAlignment(HAlign_Fill);
				slot->SetVerticalAlignment(VAlign_Fill);
				IconImages.Add(iconImage);
			}
		}
		else
		{
			IconImages[i]->SetVisibility(ESlateVisibility::Visible);
			if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *data[i].IconImagePath))
			{
				IconImages[i]->GetDynamicMaterial()->SetTextureParameterValue(FName("IconImage"), texture);
			}
		}
	}
}

void USubmitFoodUI::NativeConstruct()
{
	Super::NativeConstruct();

	IconImages.Empty(4);
}
