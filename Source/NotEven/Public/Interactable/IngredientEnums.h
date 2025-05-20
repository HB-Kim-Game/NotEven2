// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IngredientEnums.generated.h"

UENUM(BlueprintType)
enum class EIngredientState : uint8
{
	// 기본
	None UMETA(DisplayName = "None"),
	// 잘라짐
	Sliced UMETA(DisplayName = "Sliced"),
	// 구워짐
	Roasted	UMETA(DisplayName = "Roasted"),
	// 삶아짐
	Boiled UMETA(DisplayName = "Boiled"),
	// 튀김
	Fried	UMETA(DisplayName = "Fried"),
};

UENUM(BlueprintType)
enum class EPlacementRequirement : uint8
{
	// 바닥에 둘 수 있음
	Ground,
	// 접시에 둘 수 있음
	Plate,
	// 프라이팬에 둘 수 있음
	Pan,
	// 솥에 둘 수 있음
	Pot,
	// 도마에 둘 수 있음
	CuttingBoard 
};