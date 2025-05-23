// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIngredient.h"

#include "IngredientActorIcon.h"
#include "NotEvenPlayer.h"
#include "CookingProgress.h"
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
	BoxComp->SetBoxExtent(FVector(18.f, 30.f, 10.f));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	MeshComp->SetCollisionProfileName(FName("OverlapAllDynamic"));

	IconWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("IconWidgetComp"));
	IconWidgetComp->SetupAttachment(BoxComp);

	ProgressWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidgetComp"));
	ProgressWidgetComp->SetupAttachment(BoxComp);

	ConstructorHelpers::FClassFinder<UIngredientActorIcon> iconClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_IngredientActorIcon.WBP_IngredientActorIcon_C'"));

	if (iconClass.Succeeded())
	{
		IconClass = iconClass.Class;
	}
	
	IconWidgetComp->SetWidgetClass(IconClass);
	
	ConstructorHelpers::FClassFinder<UCookingProgress> progressClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_CookingProgress.WBP_CookingProgress_C'"));

	if (progressClass.Succeeded())
	{
		ProgressClass = progressClass.Class;
	}

	ProgressWidgetComp->SetWidgetClass(ProgressClass);
}

void AFoodIngredient::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(2.5f, 2.5f, 2.5f));
	
	IconWidget = Cast<UIngredientActorIcon>(IconWidgetComp->GetWidget());
	IconWidgetComp->SetDrawSize(FVector2D(20.f, 20.f));
	IconWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	ProgressWidget = Cast<UCookingProgress>(ProgressWidgetComp->GetWidget());
	
	ProgressWidgetComp->SetDrawSize(FVector2D(55.f, 10.f));
	ProgressWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	
	PlayerCameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
}

void AFoodIngredient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (CurrentState != EIngredientState::None)
	{
		IconWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 300.f));
		IconWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(IconWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));
	}
	
	if (ProgressWidget->GetVisibility() != ESlateVisibility::Hidden)
	{
		ProgressWidgetComp->SetWorldLocation(FVector(GetActorLocation().X, GetActorLocation().Y, CurrentState != EIngredientState::None ? GetActorLocation().Z + 400.f : GetActorLocation().Z + 300.f));
		ProgressWidgetComp->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(ProgressWidgetComp->GetComponentLocation(), PlayerCameraManager->GetCameraLocation()));	
	}
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
			IngredientMeshes.Add(path.State, mesh);	
		}
	}

	SetMaxCookingProgress(Data.MaxCookingProgress);

	SetState(EIngredientState::None);

	if (IconWidget)
	{
		IconWidget->SetIconImage(nullptr);
	}
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

	ProgressWidget->SetVisibility(ESlateVisibility::Visible);
	ProgressWidget->Progress = CurrentCookingProgress / MaxCookingProgress;
}

float AFoodIngredient::GetCurrentCookingProgress() const
{
	return CurrentCookingProgress;
}

float AFoodIngredient::GetMaxCookingProgress() const
{
	return MaxCookingProgress;
}

void AFoodIngredient::SetState(EIngredientState newState)
{
	CurrentState = newState;

	if (CurrentState != EIngredientState::None)
	{
		if (IconWidget)
		{
			UE_LOG(LogTemp,Warning, TEXT("Name : %s"), *Data.IconAssetPath);
			if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
			{
				IconWidget->SetIconImage(texture);
			}
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
	ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AFoodIngredient::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	switch (CurrentState)
	{
		case EIngredientState::None:
			AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("GrabPoint"));
			break;
		case EIngredientState::Sliced:
			AttachToComponent(player->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("GrabPoint"));
			break;
		default:
			break;
	}
}

void AFoodIngredient::SetGrab(bool bGrab)
{
	// 잡았을 경우
	if (bGrab)
	{
		BoxComp->SetSimulatePhysics(false);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	// 놓았을 경우
	else
	{
		BoxComp->SetSimulatePhysics(true);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	}
}

void AFoodIngredient::SetMaxCookingProgress(float progress)
{
	MaxCookingProgress = progress;
}

