// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager/OrderManager.h"

#include "IngredientStruct.h"
#include "OrderListViewer.h"
#include "PlayerUI.h"
#include "PlayTimeUI.h"
#include "PriceUI.h"
#include "ResultUI.h"
#include "Data/RecipeData.h"
#include "Data/ResultData.h"

// Sets default values for this component's properties
UOrderManager::UOrderManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/RecipeTable.RecipeTable'"));

	if (tempTable.Succeeded())
	{
		OrderTable = tempTable.Object;
	}

	ConstructorHelpers::FClassFinder<UPlayerUI> tempPlayerUI(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_PlayerUI.WBP_PlayerUI_C'"));

	if (tempPlayerUI.Succeeded())
	{
		PlayerUIClass = tempPlayerUI.Class;
	}
}


// Called when the game starts
void UOrderManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (OrderTable->GetRowNames().Num() > 0)
	{
		RecipeNameList = OrderTable->GetRowNames();
	}

	PlayerUI = Cast<UPlayerUI>(CreateWidget(GetWorld(), PlayerUIClass));
	if (PlayerUI)
	{
		PlayerUI->AddToViewport();
		PlayerUI->OrderListViewer->SetOrderManager(this);
		PlayerUI->PriceUI->SetOrderManager(this);
		
		PlayerUI->PlayTime->OnGameStart.Add(FSimpleDelegate::CreateLambda([this]()
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerUI OnGameStart"));
			this->AddOrder();
			
			this->GetWorld()->GetTimerManager().SetTimer(this->TimerHandle, [this]()
			{
				if (this->OrderList.Num() >= this->PlayerUI->OrderListViewer->GetSpawnItemsCount()) return;
				this->AddOrder();
			}, FMath::RandRange(5.f, 7.f), true);
		}));
		
		PlayerUI->PlayTime->OnGameEnd.Add(FSimpleDelegate::CreateLambda([this]()
		{
			this->GetWorld()->GetTimerManager().ClearTimer(this->TimerHandle);

			UResultData* data = NewObject<UResultData>();
			// 임시 스테이지ID
			data->StageID = TEXT("Stage0");
			data->ResultSuccessOrderCount = this->CurrentSuccessOrder;
			data->SuccessScore = this->SuccessScore;
			data->TipScore = this->TipScore;
			data->ResultFailureOrderCount = this->CurrentFailedOrder;
			data->FailureScore = this->FailureScore;
			data->ResultScore = this->CurrentScore;
			
			if (this->PlayerUI->ResultUI)
			{
				// Result UI에 정보보내기
				this->PlayerUI->ResultUI->SetVisibility(ESlateVisibility::Visible);
				this->PlayerUI->ResultUI->ShowResult(data);
				this->GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
				this->GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
			}
		}));

		// 임시
		PlayerUI->PlayTime->SetMaxTime(60.f);
		PlayerUI->PriceUI->ShowCurrentScore();
	}

}


// Called every frame
void UOrderManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

int32 UOrderManager::GetCurrentScore() const
{
	return CurrentScore;
}

int32 UOrderManager::GetCurrentComboCount() const
{
	return CurrentComboCount;
}

void UOrderManager::AddScore(int32 addScore)
{
	CurrentScore = FMath::Max(CurrentScore + addScore, 0);
}

void UOrderManager::AddSuccess(int32 price)
{
	CurrentSuccessOrder += 1;
	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 0, 4);
	SuccessScore += price;
	TipScore += FMath::Max(price * (CurrentComboCount - 1), 0);
}

void UOrderManager::AddFailure(int32 price)
{
	CurrentFailedOrder += 1;
	CurrentComboCount = 0;
	FailureScore += CurrentScore > price ? price : CurrentScore;
}

void UOrderManager::AddOrder()
{
	if (nullptr == OrderTable) return;
	if (RecipeNameList.Num() <= 0) return;

	FName randomName = RecipeNameList[FMath::RandRange(0, RecipeNameList.Num() - 1)];
	
	if (FRecipe* data = OrderTable->FindRow<FRecipe>(randomName, FString("")))
	{
		auto* order = NewObject<URecipeData>();
		order->RecipeID = data->RecipeID;
		order->DisplayName = data->DisplayName;
		order->Price = data->Price;
		order->MaxCookingTime = data->MaxCookingTime;
		order->CurrentCookingTime = data->MaxCookingTime;
		order->RequiredIngredients = data->RequiredIngredients;
		order->IconImagePath = data->IconImagePath;

		OrderList.Add(order);
	}

	PlayerUI->OrderListViewer->FetchDatas<URecipeData>(OrderList);
}

void UOrderManager::RefreshOrder()
{
	PlayerUI->OrderListViewer->FetchDatas<URecipeData>(OrderList);
}

TArray<class URecipeData*> UOrderManager::RemoveOrder(URecipeData* data, bool isSuccess)
{
	int32 index = OrderList.Find(data);
	if (index != INDEX_NONE)
	{
		OrderList.RemoveAt(index);
	}
	
	return OrderList;
}

