// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LoadingUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/NotEvenSave.h"
#include "Data/ResultData.h"
#include "Kismet/GameplayStatics.h"

ULoadingUI::ULoadingUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/StageScoreTable.StageScoreTable'"));

	if (tempTable.Succeeded())
	{
		ScoreTable = tempTable.Object;
	}
}

void ULoadingUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void ULoadingUI::ShowStageInfo()
{
	StarOneImage->SetVisibility(ESlateVisibility::Collapsed);
	StarTwoImage->SetVisibility(ESlateVisibility::Collapsed);
	StarThreeImage->SetVisibility(ESlateVisibility::Collapsed);

	if (FStageRequireScore* find = ScoreTable->FindRow<FStageRequireScore>(FName("Stage0"), FString("")))
	{
		StageText->SetText(FText::FromString(find->DisplayName));

		if (auto Save = Cast<UNotEvenSave>(UGameplayStatics::LoadGameFromSlot(TEXT("Stage0"), 0)))
		{
			if (Save->HighScore >= find->RequireScores[0]) StarOneImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (Save->HighScore >= find->RequireScores[1]) StarTwoImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (Save->HighScore >= find->RequireScores[2]) StarThreeImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			BestScoreText->SetText(FText::FromString(FString::FromInt(Save->HighScore)));
		}
		else
		{
			BestScoreText->SetText(FText::FromString(TEXT("0")));
		}

		StarOneText->SetText(FText::FromString(FString::FromInt(find->RequireScores[0])));
		
		StarTwoText->SetText(FText::FromString(FString::FromInt(find->RequireScores[1])));
		
		StarThreeText->SetText(FText::FromString(FString::FromInt(find->RequireScores[2])));

	}
}
