// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/SubmitFood.h"
#include "Components/WidgetComponent.h"
#include "SubmitFoodUI.h"
#include "Kismet/KismetMathLibrary.h"

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

	IconWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidgetComp"));
	IconWidgetComp->SetupAttachment(BoxComp);

	BoxComp->SetCollisionProfileName(FName("Food"));
	ConstructorHelpers::FClassFinder<USubmitFoodUI> iconClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_SubmitFoodUI.WBP_SubmitFoodUI_C'"));

	if (iconClass.Succeeded())
	{
		IconClass = iconClass.Class;
	}
	
	IconWidgetComp->SetWidgetClass(IconClass);

	bReplicates = true;
}

void ASubmitFood::AddIngredient(FIngredientData data, EIngredientState state, float CookingProgress, FIngredientPlaceData placeData)
{
	if (Ingredients.Num() >= 4) return;

	UE_LOG(LogTemp, Warning, TEXT("Add"));
	
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

	if (!IconWidget)
	{
		IconWidget = Cast<USubmitFoodUI>(IconWidgetComp->GetWidget());
	}

	IconWidget->ShowIconImage(GetIngredients());
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
	
	IconWidgetComp->SetDrawAtDesiredSize(true);
	IconWidgetComp->SetDrawSize(FVector2D(150.f, 150.f));
	IconWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	
	PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

void ASubmitFood::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	IconWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 200.f));
	IconWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(IconWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));
}

void ASubmitFood::FindMesh()
{
	TArray<FSubmitFoodMeshData*> array;
	MeshTable->GetAllRows<FSubmitFoodMeshData>(nullptr, array);

	UE_LOG(LogTemp, Warning, TEXT("Count : %d"), array.Num());
	
	TArray<FRecipeIngredientData> temp = GetIngredients();
	
	if (auto find = array.FindByPredicate([this, temp](const FSubmitFoodMeshData* item)
	{
		UE_LOG(LogTemp,Warning,TEXT("ASubmitFood::[%s]"), *item->MeshAssetPath);
		bool condition = true;
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
