// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ListViewerBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UListViewerBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOTEVEN_API IListViewerBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int32 GetCursor() const = 0;

	virtual void SetCursor(int32 value) = 0;

	virtual UObject* GetSelectedItem() const = 0;

	virtual void OnDataFetched() = 0;

	virtual void MoveCursor(int32 Gap, bool bIsRefresh = false) = 0;
	
};
