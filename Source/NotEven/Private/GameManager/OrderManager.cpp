// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager/OrderManager.h"
#include "IngredientStruct.h"
#include "KitchenSink.h"
#include "KitchenTable.h"
#include "OrderListViewer.h"
#include "PlayerUI.h"
#include "PlayTimeUI.h"
#include "PriceUI.h"
#include "ResultUI.h"
#include "Stove.h"
#include "Data/RecipeData.h"
#include "Data/ResultData.h"
#include "Kismet/GameplayStatics.h"
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

	if (HasAuthority())
	{
		OnGameStart.Add(FSimpleDelegate::CreateLambda([this]()
		{
			UE_LOG(LogTemp,Warning,TEXT("PlayerUI OnGameStart"));
			this->AddOrder();
			this->GetWorld()->GetTimerManager().SetTimer(this->TimerHandle, [this]()
			{
				if (this->OrderList.Num() >= this->PlayerUI->OrderListViewer->GetSpawnItemsCount()) return;
				this->AddOrder();
			}, FMath::RandRange(5.f, 7.f), true);

			TArray<AActor*> tables;
			UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AKitchenTable::StaticClass(), tables);
			for (AActor* actor : tables)
			{
				if (auto table = Cast<AKitchenTable>(actor))
				{
					table->Server_SpawnObject();
				}
			}

			TArray<AActor*> stoves;
			UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AStove::StaticClass(), stoves);
			for (AActor* actor : stoves)
			{
				if (auto stove = Cast<AStove>(actor))
				{
					stove->Server_SpawnObject();
				}
			}
			
		}));
		
		OnGameEnd.Add(FSimpleDelegate::CreateLambda([&]()
		{
			this->GetWorld()->GetTimerManager().ClearTimer(this->TimerHandle);
			NetRPC_ShowResult(CurrentSuccessOrder, CurrentFailedOrder, CurrentScore, SuccessScore, TipScore, FailureScore);
		}));

		OnFinishedTimeOverAnim.BindDynamic(this, &AOrderManager::GameEnd);
		OnFinishedStartAnim.BindDynamic(this, &AOrderManager::GameStart);	
	}
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

void AOrderManager::AddSuccess_Implementation(int32 price)
{
	CurrentSuccessOrder += 1;
	SuccessScore += price;
}

void AOrderManager::AddScore_Implementation(int32 addScore)
{
	CurrentScore = FMath::Max(CurrentScore + addScore, 0);
	CurrentComboCount = addScore > 0 ? FMath::Clamp(CurrentComboCount + 1, 0, 4) : 0;
	TipScore += FMath::Max(8.f * CurrentComboCount, 0);

	ShowCurrentScore(CurrentScore, CurrentComboCount);
}

void AOrderManager::ShowCurrentScore_Implementation(int32 currentScore, int32 currentComboCount)
{
	CurrentScore = currentScore;
	CurrentComboCount = currentComboCount;
	
	if (PlayerUI)
	{
		PlayerUI->PriceUI->ShowCurrentScore();
	}
}

void AOrderManager::AddOrder_Implementation()
{
	if (nullptr == OrderTable) return;
	if (RecipeNameList.Num() <= 0) return;

	FName randomName = RecipeNameList[FMath::RandRange(0, RecipeNameList.Num() - 1)];
	
	if (FRecipe* data = OrderTable->FindRow<FRecipe>(randomName, FString("")))
	{
		CreateOrderObject(*data);
	}
}

void AOrderManager::CreateOrderObject_Implementation(const FRecipe& order)
{
	auto* recipeData = NewObject<URecipeData>();
	recipeData->RecipeID = order.RecipeID;
	recipeData->DisplayName = order.DisplayName;
	recipeData->Price = order.Price;
	recipeData->MaxCookingTime = order.MaxCookingTime;
	recipeData->CurrentCookingTime = order.MaxCookingTime;
	recipeData->RequiredIngredients = order.RequiredIngredients;
	recipeData->IconImagePath = order.IconImagePath;

	OrderList.Add(recipeData);

	PlayerUI->OrderListViewer->FetchDatas<URecipeData>(OrderList);
}

void AOrderManager::AddFailure_Implementation(int32 price)
{
	CurrentFailedOrder += 1;
	FailureScore += CurrentScore > price ? price : CurrentScore;
}

void AOrderManager::RefreshOrder()
{
	PlayerUI->OrderListViewer->FetchDatas<URecipeData>(OrderList);
}

void AOrderManager::CheckOrder_Implementation(const TArray<struct FRecipeIngredientData>& ingredients)
{
	NetRPC_CheckOrder(ingredients);
}

void AOrderManager::NetRPC_CheckOrder_Implementation(const TArray<struct FRecipeIngredientData>& ingredients)
{
	PlayerUI->OrderListViewer->CheckOrderSuccess(ingredients);
}

void AOrderManager::NetRPC_RemoveOrder_Implementation(int32 index)
{
	if (index != INDEX_NONE && index < OrderList.Num())
	{
		OrderList.RemoveAt(index);
	}
}

void AOrderManager::NetRPC_ShowResult_Implementation(int32 successOrder, int32 failedOrder, int32 currentScore,
	int32 successScore, int32 tipScore, int32 failureScore)
{
	UResultData* data = NewObject<UResultData>();
	// 임시 스테이지ID
	data->StageID = TEXT("Stage0");
	data->ResultSuccessOrderCount = successOrder;
	data->SuccessScore = successScore;
	data->TipScore = tipScore;
	data->ResultFailureOrderCount = failedOrder;
	data->FailureScore = failureScore;
	data->ResultScore = currentScore;
			
	if (PlayerUI->ResultUI)
	{
		// Result UI에 정보보내기
		PlayerUI->ResultUI->SetVisibility(ESlateVisibility::Visible);
		PlayerUI->ResultUI->ShowResult(data);
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
	}
}

float AOrderManager::GetMaxTime() const
{
	return MaxTime;
}

void AOrderManager::InitWidget()
{
	if (PlayerUI) return;
	
	PlayerUI = Cast<UPlayerUI>(CreateWidget(GetWorld(), PlayerUIClass));
	if (PlayerUI)
	{
		PlayerUI->AddToViewport();	
		PlayerUI->OrderListViewer->SetOrderManager(this);
		PlayerUI->PriceUI->SetOrderManager(this);
		PlayerUI->PlayTime->BindToAnimationFinished(PlayerUI->PlayTime->TimeOver, OnFinishedTimeOverAnim);
		
		// 임시
		PlayerUI->PlayTime->SetMaxTime(this);
		PlayerUI->PriceUI->ShowCurrentScore();
		PlayerUI->BindToAnimationFinished(PlayerUI->Start, OnFinishedStartAnim);

		PlayerUI->GameStart();
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

void AOrderManager::GameStart()
{
	IsPlaying = true;
	OnGameStart.Broadcast();
}

TArray<class URecipeData*> AOrderManager::RemoveOrder(URecipeData* data, bool isSuccess)
{
	int32 index = OrderList.Find(data);

	NetRPC_RemoveOrder(index);
	
	return OrderList;
}

