// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/FoodFactory.h"

#include "ConveyorBelt.h"
#include "FoodIngredient.h"
#include "Components/BoxComponent.h"
#include "GameManager/OrderManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFoodFactory::AFoodFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	ConstructorHelpers::FObjectFinder<UDataTable> tempIngredientTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/IngredientTable.IngredientTable'"));

	if (tempIngredientTable.Succeeded())
	{
		IngredientTable = tempIngredientTable.Object;
	}

	ConstructorHelpers::FObjectFinder<UDataTable> tempPlaceTable(TEXT("/Script/Engine.DataTable'/Game/KHB/DataTable/PlacementTable.PlacementTable'"));

	if (tempPlaceTable.Succeeded())
	{
		PlaceTable = tempPlaceTable.Object;
	}

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(100, 100, 50));
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp -> SetupAttachment(BoxComp);
	MeshComp->SetRelativeLocation(FVector(0,0,-50));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/fridge_A_decorated.fridge_A_decorated'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void AFoodFactory::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		auto orderManager = UGameplayStatics::GetActorOfClass(GetWorld(), AOrderManager::StaticClass());

		auto cast = Cast<AOrderManager>(orderManager);
		
		cast->OnGameStart.Add(FSimpleDelegate::CreateLambda([&]()
		{
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				auto food = GetWorld()->SpawnActor<AFoodIngredient>();
				FIngredientData ingredientData;
				FIngredientPlaceData placeData;
				FString foodName = IngredientIDs[FMath::RandRange(0, IngredientIDs.Num()-1)];

				if (IngredientTable->GetRowNames().Num() > 0)
				{
					if (FIngredientData* data = IngredientTable->FindRow<FIngredientData>(FName(foodName), FString("")))
					{
						ingredientData = *data;
					}
				}

				if (PlaceTable->GetRowNames().Num() > 0)
				{
					if (FIngredientPlaceData* data = PlaceTable->FindRow<FIngredientPlaceData>(FName(foodName), FString("")))
					{
						placeData = *data;
					}
				}

				food->InitializeIngredient(ingredientData, placeData);
				food->BoxComp->SetSimulatePhysics(false);
				food->AttachToComponent(StartBelt->OverlapBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				if (StartBelt->OwnedObj.Contains(food)) return;
				StartBelt->OwnedObj.Add(food);
			}, FMath::RandRange(5.f, 7.f), true);
		}));
		
		cast->OnGameEnd.Add(FSimpleDelegate::CreateLambda([&]()
		{
			GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
		}));
	}
}

// Called every frame
void AFoodFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

