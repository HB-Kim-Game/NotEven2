// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "EvenUtils.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UEvenUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
#pragma region 리스트 Clamp 함수, Max는 exclude
	UFUNCTION(BlueprintCallable, Category = "Math")
	static int32 ClampCursor(int32 value, int32 max, int min = 0);
#pragma endregion
};
