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

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/LGJ/Assets/CookingAssets/fbx/fridge_A_decorated.fridge_A_decorated'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}
	MeshComp->SetRelativeScale3D(FVector(0.8f, 0.8f, 0.8f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> iconMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));

	IconMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("IconMesh"));

	IconMesh->SetupAttachment(BoxComp);

	if (iconMesh.Succeeded())
	{
		IconMesh->SetStaticMesh(iconMesh.Object);
	}

	IconMesh->SetRelativeLocationAndRotation(FVector(0.f, -20.f, 150.f),FRotator(0.f, 0.f, 0.f));
	IconMesh->SetRelativeScale3D(FVector(0.75f));

	ConstructorHelpers::FObjectFinder<UMaterialInterface> iconMat(TEXT("/Script/Engine.MaterialInstanceConstant'/Game/KHB/UI/M_CircleMaskNotUI_Inst.M_CircleMaskNotUI_Inst'"));
	if (iconMat.Succeeded())
	{
		IconMaterial = iconMat.Object;
	}
}

void AIngredientBox::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
	NetRPC_Interact(player);
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

	IconDynamic = UMaterialInstanceDynamic::Create(IconMaterial, this);
	UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath);
	IconDynamic->SetTextureParameterValue(FName("IconImage"), texture);
	IconMesh->SetMaterial(0, IconDynamic);

}

// Called every frame
void AIngredientBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AIngredientBox::NetRPC_Interact_Implementation(class ANotEvenPlayer* player)
{
	auto* ingredient = GetWorld()->SpawnActor<AFoodIngredient>(AFoodIngredient::StaticClass(),
		FVector(player->GetActorLocation().X, player->GetActorLocation().Y, player->GetActorLocation().Z + 200.f), FRotator::ZeroRotator);

	ingredient->InitializeIngredient(Data, PlaceData);
	
	player->AttachGrabObj(ingredient);
	ingredient->Interact(player);
}

