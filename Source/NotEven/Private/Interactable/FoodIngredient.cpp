// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIngredient.h"

#include "IngredientActorIcon.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetMathLibrary.h"

AFoodIngredient::AFoodIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	BoxComp->SetSimulatePhysics(true);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	BoxComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	BoxComp->SetBoxExtent(FVector(30.f, 30.f, 10.f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	MeshComp->SetCollisionProfileName(FName("OverlapAllDynamic"));

	IconWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidgetComp"));
	IconWidgetComp->SetupAttachment(BoxComp);

	ConstructorHelpers::FClassFinder<UIngredientActorIcon> iconClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_IngredientActorIcon.WBP_IngredientActorIcon_C'"));

	if (iconClass.Succeeded())
	{
		IconClass = iconClass.Class;
	}
	
	IconWidgetComp->SetWidgetClass(IconClass);
}

void AFoodIngredient::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(2.5f, 2.5f, 2.5f));
	
	IconWidget = Cast<UIngredientActorIcon>(IconWidgetComp->GetWidget());
	IconWidgetComp->SetDrawSize(FVector2D(20.f, 20.f));
	IconWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	
	PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	
	// if (IconWidget)
	// {
	// 	IconWidget->SetIconImage(nullptr);
	// }
}

void AFoodIngredient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	IconWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 300.f));
	IconWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(IconWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));
}

void AFoodIngredient::InitializeIngredient(FIngredientData data, FIngredientPlaceData place)
{
	Data = data;
	PlaceData = place;

	if (Data.MeshPaths.Num() <= 0) return;
	
	for (auto path : Data.MeshPaths)
	{
		if (UStaticMesh* mesh = LoadObject<UStaticMesh>(nullptr, *path.StaticMeshPath))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *path.StaticMeshPath);
			IngredientMeshes.Add(path.State, mesh);	
		}
	}

	SetState(EIngredientState::Sliced);
}

FIngredientData AFoodIngredient::GetIngredientData() const
{
	return Data;
}

EIngredientState AFoodIngredient::GetIngredientState() const
{
	return CurrentState;
}

TArray<FIngredientPlaceRule> AFoodIngredient::GetIngredientPlaceRules() const
{
	return PlaceData.PlacementRules;
}

void AFoodIngredient::AddCookingProgress(float addProgress)
{
	CurrentCookingProgress += addProgress;
}

float AFoodIngredient::GetNormalizedCookingProgress() const
{
	return CurrentCookingProgress / MaxCookingProgress;
}

void AFoodIngredient::SetState(EIngredientState newState)
{
	CurrentState = newState;

	if (CurrentState != EIngredientState::None)
	{
		if (IconWidget)
		{
			// if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
			// {
				// IconWidget->SetIconImage(texture);
			// }
		}
	}

	if (auto mesh = IngredientMeshes.Find(newState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);	
		}
	}
	
	CurrentCookingProgress = 0.0f;
}

void AFoodIngredient::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	
}

