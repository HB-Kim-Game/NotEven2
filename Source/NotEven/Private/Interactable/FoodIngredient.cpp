// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodIngredient.h"

#include "IngredientActorIcon.h"
#include "NotEvenPlayer.h"
#include "CookingProgress.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

AFoodIngredient::AFoodIngredient()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp->SetCollisionProfileName(FName("Food"));
	ConstructorHelpers::FClassFinder<UIngredientActorIcon> iconClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_IngredientActorIcon.WBP_IngredientActorIcon_C'"));

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

void AFoodIngredient::BeginPlay()
{
	Super::BeginPlay();

	BoxComp->SetSimulatePhysics(false);

	IconWidget = CreateWidget<UIngredientActorIcon>(GetWorld(), IconClass);
	IconWidget->AddToViewport();
	IconWidget->SetDesiredSizeInViewport(FVector2D(32.f, 32.f));
	IconWidget->SetVisibility(ESlateVisibility::Hidden);

	ProgressWidget = CreateWidget<UCookingProgress>(GetWorld(), ProgressClass);
	ProgressWidget->AddToViewport();
	ProgressWidget->SetDesiredSizeInViewport(FVector2D(75.f, 15.f));
	ProgressWidget->SetVisibility(ESlateVisibility::Hidden);

	OnDestroyed.AddDynamic(this, &AFoodIngredient::RemoveWidget);
}

void AFoodIngredient::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector2D screenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), GetActorLocation(), screenPosition);

	FVector2D iconPosition = screenPosition;
	iconPosition.X = iconPosition.X - 32.0f;
	iconPosition.Y = iconPosition.Y - 125.f;
	
	IconWidget->SetPositionInViewport(iconPosition);
	
	if (ProgressWidget->GetVisibility() != ESlateVisibility::Hidden)
	{
		if (CurrentState != EIngredientState::None)
		{
			FVector2D progressPosition = screenPosition;
			progressPosition.X = progressPosition.X - 75.0f;
			progressPosition.Y = progressPosition.Y + 75.f;
	
			ProgressWidget->SetPositionInViewport(progressPosition);
		}
		else
		{
			FVector2D progressPosition = screenPosition;
			progressPosition.X = progressPosition.X - 75.0f;
			progressPosition.Y = progressPosition.Y - 75.f;
	
			ProgressWidget->SetPositionInViewport(progressPosition);
		}
	}
}

void AFoodIngredient::InitializeIngredient_Implementation(FIngredientData data, FIngredientPlaceData place)
{
	Data = data;
	PlaceData = place;
	
	NetMulticast_InitializeIngredient(Data, PlaceData);
}

void AFoodIngredient::NetMulticast_InitializeIngredient_Implementation(FIngredientData data, FIngredientPlaceData place)
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

	CurrentState = EIngredientState::None;

	if (!IconWidget)
	{
		IconWidget = CreateWidget<UIngredientActorIcon>(GetWorld(), IconClass);
		IconWidget->AddToViewport();
		IconWidget->SetDesiredSizeInViewport(FVector2D(32.f, 32.f));
		IconWidget->SetVisibility(ESlateVisibility::Hidden);

		ProgressWidget = CreateWidget<UCookingProgress>(GetWorld(), ProgressClass);
		ProgressWidget->AddToViewport();
		ProgressWidget->SetDesiredSizeInViewport(FVector2D(75.f, 15.f));
		ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	}
	
	IconWidget->SetVisibility(ESlateVisibility::Hidden);
	
	if (auto mesh = IngredientMeshes.Find(CurrentState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);
			BoxComp->SetBoxExtent(MeshComp->GetStaticMesh()->GetBounds().BoxExtent);
			if ((*mesh)->GetBounds().BoxExtent.Z > MeshComp->Bounds.BoxExtent.X) MeshComp->SetRelativeRotation(FRotator(90.f, 0, 0));
			else MeshComp->SetRelativeRotation(FRotator(0, 0, 0));
		}
	}
	
	if (IconWidget)
	{
		if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
		{
			IconWidget->SetIconImage(texture);
		}
	}
	
	CurrentCookingProgress = 0.0f;
	ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
	
	BoxComp->SetMassOverrideInKg(NAME_None, 150.f);
	BoxComp->SetLinearDamping(2.f);
}

FIngredientData AFoodIngredient::GetIngredientData() const
{
	return Data;
}

EIngredientState AFoodIngredient::GetIngredientState() const
{
	return CurrentState;
}

FIngredientPlaceData AFoodIngredient::GetIngredientPlaceData() const
{
	return PlaceData;
}

void AFoodIngredient::AddCookingProgress(float addProgress)
{
	CurrentCookingProgress += addProgress;
	if (HasAuthority())
	{
		Rep_CurrentCookingProgress();	
	}
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

	NetMulticast_SetCurrentState(CurrentState);
}

void AFoodIngredient::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);
	NetRPC_Interact(player);
}

void AFoodIngredient::SetGrab(bool bGrab)
{
	Super::SetGrab(bGrab);

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

void AFoodIngredient::Rep_CurrentCookingProgress()
{
	if (CurrentCookingProgress <= 0.01f)
	{
		ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	ProgressWidget->SetVisibility(ESlateVisibility::Visible);
	ProgressWidget->Progress = CurrentCookingProgress / MaxCookingProgress;
	
}

void AFoodIngredient::NetMulticast_SetCurrentState_Implementation(EIngredientState next)
{
	CurrentState = next;
	IconWidget->SetVisibility(CurrentState != EIngredientState::None ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	
	if (auto mesh = IngredientMeshes.Find(CurrentState))
	{
		if (*mesh)
		{
			MeshComp->SetStaticMesh(*mesh);
			BoxComp->SetBoxExtent(MeshComp->GetStaticMesh()->GetBounds().BoxExtent);
			if ((*mesh)->GetBounds().BoxExtent.Z > MeshComp->Bounds.BoxExtent.X) MeshComp->SetRelativeRotation(FRotator(90.f, 0, 0));
			else MeshComp->SetRelativeRotation(FRotator(0, 0, 0));
		}
	}
	
	if (IconWidget)
	{
		if (UTexture2D* texture = LoadObject<UTexture2D>(nullptr, *Data.IconAssetPath))
		{
			IconWidget->SetIconImage(texture);
		}
	}
	
	CurrentCookingProgress = 0.0f;
	ProgressWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AFoodIngredient::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFoodIngredient, CurrentCookingProgress);
}

void AFoodIngredient::RemoveWidget(AActor* DestroyedActor)
{
	if (IconWidget) IconWidget->RemoveFromParent();
	if (ProgressWidget) ProgressWidget->RemoveFromParent();
}

void AFoodIngredient::NetRPC_Interact_Implementation(class ANotEvenPlayer* player)
{
	switch (CurrentState)
	{
	case EIngredientState::None:
		SetGrab(true);
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
		break;
	case EIngredientState::Sliced:
		SetGrab(true);
		AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
		break;
	default:
		break;
	}
}