// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ViewerItemBase.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UViewerItemBase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class NOTEVEN_API IViewerItemBase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void FetchData(UObject* Data) = 0;
	virtual void Selected() = 0;
	virtual void Deselected() = 0;
};
