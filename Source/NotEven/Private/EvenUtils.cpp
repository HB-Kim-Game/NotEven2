// Fill out your copyright notice in the Description page of Project Settings.


#include "EvenUtils.h"

int32 UEvenUtils::ClampCursor(int32 value, int32 max, int min)
{
	while (value >= max) value -= max;
	while (value < min) value += max;

	return value;
}
