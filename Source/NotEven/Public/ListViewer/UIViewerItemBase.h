// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ViewerItemBase.h"
#include "UIViewerItemBase.generated.h"

/**
 * 
 */
UCLASS()
class NOTEVEN_API UUIViewerItemBase : public UUserWidget, public IViewerItemBase
{
	GENERATED_BODY()

public:
	virtual void FetchData(UObject* Data) override;
	virtual void Selected() override;
	virtual void Deselected() override;
};
