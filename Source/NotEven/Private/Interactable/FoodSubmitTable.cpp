// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/FoodSubmitTable.h"
#include "IngredientStruct.h"
#include "NotEvenGameMode.h"
#include "Plate.h"
#include "PlateTable.h"
#include "SubmitFood.h"
#include "SubmitTableUI.h"
#include "Components/WidgetComponent.h"
#include "GameManager/OrderManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AFoodSubmitTable::AFoodSubmitTable()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/KHB/Models/kitchentable_B.kitchentable_B'"));

	if (tempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(tempMesh.Object);
	}

	TextWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("TextWidgetComp"));
	TextWidgetComp->SetupAttachment(BoxComp);
	
	ConstructorHelpers::FClassFinder<USubmitTableUI> textClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_SubmitTable.WBP_SubmitTable_C'"));

	if (textClass.Succeeded())
	{
		TextClass = textClass.Class;
	}
	
	TextWidgetComp->SetWidgetClass(TextClass);

	bIsInteractable = true;
}

void AFoodSubmitTable::BeginPlay()
{
	Super::BeginPlay();

	if (!TextWidget)
	{
		TextWidget = Cast<USubmitTableUI>(TextWidgetComp->GetWidget());
	}

	TextWidgetComp->SetWorldLocation(FVector(TextWidgetComp->GetComponentLocation().X - 125.f, TextWidgetComp->GetComponentLocation().Y, TextWidgetComp->GetComponentLocation().Z + 125.f));

	OrderManager = Cast<AOrderManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AOrderManager::StaticClass()));

	plateTable = Cast<APlateTable>(UGameplayStatics::GetActorOfClass(GetWorld(), APlateTable::StaticClass()));
}

void AFoodSubmitTable::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	NetMulticast_Interact(player);
}

void AFoodSubmitTable::NetMulticast_Interact_Implementation(class ANotEvenPlayer* player)
{
	auto PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	TextWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(TextWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));

	if (auto plate = Cast<APlate>(player->OwnedObj))
	{
		if (plate->submitFood)
		{
			player->DetachGrabObj(false);
			SubmitFood(plate->submitFood->GetIngredients(), plate);
			return;
		}
	}

	TextWidget->StopAllAnimations();
	TextWidget->PlayAnimation(TextWidget->Appear);
}

void AFoodSubmitTable::SubmitFood(const TArray<struct FRecipeIngredientData>& ingredients, APlate* plate)
{
	if (HasAuthority())
	{
		OrderManager->CheckOrder(ingredients);
		GetWorld()->GetTimerManager().SetTimer(PlateSpawnTimer,FTimerDelegate::CreateLambda([&,plate]()
	{
		plateTable->MakePlate();
	}),5,false);
		
	}
	plate->submitFood->Destroy();
	plate->Destroy();
}
