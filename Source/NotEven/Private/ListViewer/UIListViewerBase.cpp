// Fill out your copyright notice in the Description page of Project Settings.


#include "UIListViewerBase.h"
#include "EvenUtils.h"
#include "UIViewerItemBase.h"

int32 UUIListViewerBase::GetCursor() const
{
	return Cursor;
}

void UUIListViewerBase::SetCursor(int32 value)
{
	Cursor = UEvenUtils::ClampCursor(value, FetchedDatas.Num());
}

UObject* UUIListViewerBase::GetSelectedItem() const
{
	return FetchedDatas[Cursor];
}

void UUIListViewerBase::MoveCursor(int32 Gap, bool bIsRefresh)
{
	SetCursor(Cursor + Gap);
}

void UUIListViewerBase::InitializeItem()
{
	for (int32 i = 0; i < ItemPoolCount; i++)
	{
		if (UUIViewerItemBase* item = Cast<UUIViewerItemBase>(CreateWidget(GetWorld(), ItemClass)))
		{
			SpawnItems.Add(item);
		}
	}
}
