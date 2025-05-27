// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultUI.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Data/NotEvenSave.h"
#include "Data/ResultData.h"
#include "Kismet/GameplayStatics.h"

UResultUI::UResultUI(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/StageScoreTable.StageScoreTable'"));

	if (tempTable.Succeeded())
	{
		ScoreTable = tempTable.Object;
	}
}

void UResultUI::ShowResult(class UResultData* data)
{
	//StageText->SetText(FText::FromString(data->DisplayName));
	SuccessOrderCountText->SetText(FText::FromString(FString::FromInt(data->ResultSuccessOrderCount)));
	SuccessScoreText->SetText(FText::FromString(FString::FromInt(data->SuccessScore)));
	TipScoreText->SetText(FText::FromString(FString::FromInt(data->TipScore)));
	
	FailureOrderCountText->SetText(FText::FromString(FString::FromInt(data->ResultFailureOrderCount)));
	FailureScoreText->SetText(FText::FromString(FString::FromInt(data->FailureScore)));
	
	ResultScoreText->SetText(FText::FromString(FString::FromInt(data->ResultScore)));

	if (FStageRequireScore* find = ScoreTable->FindRow<FStageRequireScore>(FName(data->StageID), FString("")))
	{
		StarOneText->SetText(FText::FromString(FString::FromInt(find->RequireScores[0])));
		if (data->ResultScore >= find->RequireScores[0]) StarOneImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		StarTwoText->SetText(FText::FromString(FString::FromInt(find->RequireScores[1])));
		if (data->ResultScore >= find->RequireScores[1]) StarTwoImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		
		StarThreeText->SetText(FText::FromString(FString::FromInt(find->RequireScores[2])));
		if (data->ResultScore >= find->RequireScores[2]) StarThreeImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	auto Save = Cast<UNotEvenSave>(UGameplayStatics::LoadGameFromSlot(data->StageID, 0));

	if (!Save)
	{
		auto newData = NewObject<UNotEvenSave>();
		newData->StageID = data->StageID;
		newData->HighScore = data->ResultScore;

		UGameplayStatics::SaveGameToSlot(newData, data->StageID, 0);
		return;
	}

	if (Save->HighScore > data->ResultScore) return;

	Save->HighScore = data->ResultScore;
	UGameplayStatics::SaveGameToSlot(Save, data->StageID, 0);
}

void UResultUI::NativeConstruct()
{
	Super::NativeConstruct();

	StarOneImage->SetVisibility(ESlateVisibility::Collapsed);
	StarTwoImage->SetVisibility(ESlateVisibility::Collapsed);
	StarThreeImage->SetVisibility(ESlateVisibility::Collapsed);
}
