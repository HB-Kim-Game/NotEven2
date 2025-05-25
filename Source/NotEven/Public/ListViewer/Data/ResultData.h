// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ResultData.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UResultData : public UObject
{
	GENERATED_BODY()

public:
	FString StageID;
	int32 ResultScore;
	int32 ResultSuccessOrderCount;
	int32 ResultFailureOrderCount;
};
