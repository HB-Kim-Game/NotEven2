// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/IngredientBox.h"

#include "FoodIngredient.h"
#include "IngredientStruct.h"
#include "NotEvenPlayer.h"
#include "Components/BoxComponent.h"

// Sets default values
AIngredientBox::AIngredientBox()
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
	SetRootComponent(BoxComp);

	BoxComp->SetBoxExtent(FVector(50.f, 50.f, 50.f));
	BoxComp->SetCollisionProfileName(FName("UnGrabObj"));
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
}

void AIngredientBox::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
	auto* ingredient = GetWorld()->SpawnActor<AFoodIngredient>(AFoodIngredient::StaticClass(),
		FVector(player->GetActorLocation().X, player->GetActorLocation().Y, player->GetActorLocation().Z + 200.f), FRotator::ZeroRotator);
	
	ingredient->InitializeIngredient(Data, PlaceData);
	player->OwnedObj = ingredient;
	player->isGrab = true;
	ingredient->SetOwner(player);
	ingredient->Interact(player);
}

// Called when the game starts or when spawned
void AIngredientBox::BeginPlay()
{
	Super::BeginPlay();

	if (IngredientTable->GetRowNames().Num() > 0)
	{
		if (FIngredientData* data = IngredientTable->FindRow<FIngredientData>(FName(IngredientID), FString("")))
		{
			Data = *data;
		}
	}

	if (PlaceTable->GetRowNames().Num() > 0)
	{
		if (FIngredientPlaceData* data = PlaceTable->FindRow<FIngredientPlaceData>(FName(IngredientID), FString("")))
		{
			PlaceData = *data;
		}
	}
	//
	// FTimerHandle TimerHandle;
	// GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	// {
	// 	this->Interact(Cast<ANotEvenPlayer>(this->GetWorld()->GetFirstPlayerController()->GetPawn()));
	// }, 5.0f, true);
}

// Called every frame
void AIngredientBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

