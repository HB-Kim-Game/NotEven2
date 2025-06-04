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
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
AOrderManager::AOrderManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

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

	bReplicates = true;
	SetReplicates(true);
}


// Called when the game starts
void AOrderManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (OrderTable->GetRowNames().Num() > 0)
	{
		RecipeNameList = OrderTable->GetRowNames();
	}
	
	OnGameStart.Add(FSimpleDelegate::CreateLambda([this]()
{
	UE_LOG(LogTemp,Warning,TEXT("PlayerUI OnGameStart"));
	this->AddOrder();
			
	this->GetWorld()->GetTimerManager().SetTimer(this->TimerHandle, [this]()
	{
		if (this->OrderList.Num() >= this->PlayerUI->OrderListViewer->GetSpawnItemsCount()) return;
		this->AddOrder();
	}, FMath::RandRange(5.f, 7.f), true);
}));
		
	OnGameEnd.Add(FSimpleDelegate::CreateLambda([this]()
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

	OnFinishedTimeOverAnim.BindDynamic(this, &AOrderManager::GameEnd);
}


// Called every frame
void AOrderManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsPlaying) return;

	if (HasAuthority())
	{
		ServerRPC_SetCurrentTime(DeltaTime);	
	}
}

int32 AOrderManager::GetCurrentScore() const
{
	return CurrentScore;
}

int32 AOrderManager::GetCurrentComboCount() const
{
	return CurrentComboCount;
}

void AOrderManager::AddScore(int32 addScore)
{
	CurrentScore = FMath::Max(CurrentScore + addScore, 0);
}

void AOrderManager::AddSuccess(int32 price)
{
	CurrentSuccessOrder += 1;
	CurrentComboCount = FMath::Clamp(CurrentComboCount + 1, 0, 4);
	SuccessScore += price;
	TipScore += FMath::Max(8.f * CurrentComboCount, 0);
}

void AOrderManager::AddFailure(int32 price)
{
	CurrentFailedOrder += 1;
	CurrentComboCount = 0;
	FailureScore += CurrentScore > price ? price : CurrentScore;
}

void AOrderManager::AddOrder()
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

void AOrderManager::RefreshOrder()
{
	PlayerUI->OrderListViewer->FetchDatas<URecipeData>(OrderList);
}

bool AOrderManager::CheckOrder(const TArray<struct FRecipeIngredientData>& ingredients)
{
	return PlayerUI->OrderListViewer->CheckOrderSuccess(ingredients);
}

float AOrderManager::GetMaxTime() const
{
	return MaxTime;
}

void AOrderManager::InitWidget()
{
	PlayerUI = Cast<UPlayerUI>(CreateWidget(GetWorld(), PlayerUIClass));
	if (PlayerUI)
	{
		PlayerUI->AddToViewport();	
		PlayerUI->OrderListViewer->SetOrderManager(this);
		PlayerUI->PriceUI->SetOrderManager(this);
		PlayerUI->PlayTime->BindToAnimationFinished(PlayerUI->PlayTime->TimeOver, OnFinishedTimeOverAnim);
		
		// 임시
		PlayerUI->PlayTime->SetMaxTime(this);
		IsPlaying = true;
		PlayerUI->PriceUI->ShowCurrentScore();
	}
}

void AOrderManager::ServerRPC_SetCurrentTime_Implementation(float DeltaTime)
{
	CurrentTime -= DeltaTime;

	if (CurrentTime <= 0.f)
	{
		IsPlaying = false;
	}

	NetRPC_ShowCurrentTime(CurrentTime);
}

void AOrderManager::NetRPC_ShowCurrentTime_Implementation(float curTime)
{
	CurrentTime = curTime;
	
	if (PlayerUI)
	{
		PlayerUI->PlayTime->ShowRemainTime(CurrentTime);
	}
}

void AOrderManager::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AOrderManager, CurrentTime);
	DOREPLIFETIME(AOrderManager, IsPlaying);
}

void AOrderManager::GameEnd()
{
	OnGameEnd.Broadcast();
}

TArray<class URecipeData*> AOrderManager::RemoveOrder(URecipeData* data, bool isSuccess)
{
	int32 index = OrderList.Find(data);
	if (index != INDEX_NONE)
	{
		OrderList.RemoveAt(index);
	}
	
	return OrderList;
}

