// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SubmitFood.h"
#include "Components/WidgetComponent.h"
#include "SubmitFoodUI.h"
#include "CookingProgress.h"
#include "IngredientActorIcon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

ASubmitFood::ASubmitFood()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UDataTable> tempTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/SubmitMeshTable.SubmitMeshTable'"));

	if (tempTable.Succeeded())
	{
		MeshTable = tempTable.Object;
	}

	BoxComp->SetCollisionProfileName(FName("Food"));
	ConstructorHelpers::FClassFinder<USubmitFoodUI> iconClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_SubmitFoodUI.WBP_SubmitFoodUI_C'"));

	if (iconClass.Succeeded())
	{
		IconClass = iconClass.Class;
	}
	
	ConstructorHelpers::FClassFinder<UCookingProgress> progressClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_CookingProgress.WBP_CookingProgress_C'"));

	if (progressClass.Succeeded())
	{
		ProgressClass = progressClass.Class;
	}

	bReplicates = true;
}

void ASubmitFood::AddIngredient(FIngredientData data, EIngredientState state, float CookingProgress, FIngredientPlaceData placeData)
{
	if (Ingredients.Num() >= 4) return;
	
	FSubmitFoodIngredientData add = FSubmitFoodIngredientData();
	
	FRecipeIngredientData temp = FRecipeIngredientData();
	temp.IngredientID = data.IngredientID;
	temp.RequireState = state;
	temp.IconImagePath = data.IconAssetPath;

	add.IngredientData = temp;
	add.PlacementData = placeData;
	
	CurrentCookingProgress += CookingProgress;
	MaxCookingProgress += data.MaxCookingProgress;
	
	Ingredients.Add(add);

	FindMesh();

	IconWidget->ShowIconImage(GetIngredients());

	AddProgress(0.000001f);
}

TArray<FRecipeIngredientData> ASubmitFood::GetIngredients() const
{
	TArray<FRecipeIngredientData> temp;
	for (auto i : Ingredients)
	{
		temp.Add(i.IngredientData);
	}
	
	return temp;
}

void ASubmitFood::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
}

void ASubmitFood::BeginPlay()
{
	Super::BeginPlay();
	
	SetReplicateMovement(true);

	BoxComp->SetSimulatePhysics(false);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	IconWidget = CreateWidget<USubmitFoodUI>(GetWorld(), IconClass);
	IconWidget->AddToViewport();
	IconWidget->SetDesiredSizeInViewport(FVector2D(75.f, 75.f));

	ProgressWidget = CreateWidget<UCookingProgress>(GetWorld(), ProgressClass);
	ProgressWidget->AddToViewport();
	ProgressWidget->SetDesiredSizeInViewport(FVector2D(75.f, 15.f));
	ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	
	OnDestroyed.AddDynamic(this, &ASubmitFood::RemoveWidget);
}

void ASubmitFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector2D screenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), GetActorLocation(), screenPosition);

	FVector2D iconPosition = screenPosition;
	iconPosition.X = iconPosition.X - 50.0f;
	iconPosition.Y = iconPosition.Y - 75.f;
	
	IconWidget->SetPositionInViewport(iconPosition);
	
	FVector2D progressPosition = screenPosition;
	progressPosition.X = progressPosition.X - 75.0f;
	progressPosition.Y = progressPosition.Y + 75.f;
	
	ProgressWidget->SetPositionInViewport(progressPosition);
}

void ASubmitFood::AddProgress(float progress)
{
	if (!HasAuthority()) return;
	
	CurrentCookingProgress += progress;

	OnRep_CurrentCookingProgress();
}

void ASubmitFood::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASubmitFood, CurrentCookingProgress);
	DOREPLIFETIME(ASubmitFood, MaxCookingProgress);
}

float ASubmitFood::GetCurrentCookingProgress() const
{
	return CurrentCookingProgress;
}

float ASubmitFood::GetMaxCookingProgress() const
{
	return MaxCookingProgress;
}

void ASubmitFood::SetState(EIngredientState next)
{
	int ingredientsNum = Ingredients.Num();
	for (int i = 0; i < ingredientsNum; i++)
	{
		Ingredients[i].IngredientData.RequireState = next;
	}
	
	FindMesh();
}


void ASubmitFood::OnRep_CurrentCookingProgress()
{
	// ProgressBar 표시
	if (ProgressWidget)
	{
		if (CurrentCookingProgress <= 0.01f)
		{
			ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
			return;
		}
		ProgressWidget->SetVisibility(ESlateVisibility::Visible);
		ProgressWidget->Progress = CurrentCookingProgress / MaxCookingProgress;
	}
}

void ASubmitFood::FindMesh()
{
	TArray<FSubmitFoodMeshData*> array;
	MeshTable->GetAllRows<FSubmitFoodMeshData>(nullptr, array);
	
	TArray<FRecipeIngredientData> temp = GetIngredients();
	
	if (auto find = array.FindByPredicate([this, temp](const FSubmitFoodMeshData* item)
	{
		bool condition = false;
		for (auto i : temp)
		{
			condition = item->Ingredients.ContainsByPredicate([i](const FRecipeIngredientData& recipe)
			{
				return i.IngredientID == recipe.IngredientID && i.RequireState == recipe.RequireState;
			});
		}
		
		return condition;
	}))
	{
		UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *((*find)->MeshAssetPath));
		MeshComp->SetStaticMesh(mesh);
	}
}

void ASubmitFood::RemoveWidget(AActor* DestroyedActor)
{
	if (IconWidget) IconWidget->RemoveFromParent();
	if (ProgressWidget) ProgressWidget->RemoveFromParent();
}