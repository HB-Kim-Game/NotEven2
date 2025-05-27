// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResultData.generated.h"

/**
 *
 * 
 */

USTRUCT(Atomic, BlueprintType)
struct FStageRequireScore : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	// ...
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StageID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> RequireScores;
};

UCLASS()
class NOTEVEN_API UResultData : public UObject
{
	GENERATED_BODY()

public:
	FString StageID;
	FString DisplayName;
	int32 ResultScore;
	int32 ResultSuccessOrderCount;
	int32 SuccessScore;
	int32 TipScore;
	int32 ResultFailureOrderCount;
	int32 FailureScore;
};
