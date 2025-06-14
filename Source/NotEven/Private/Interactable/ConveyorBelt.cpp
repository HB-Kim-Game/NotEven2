// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactable/ConveyorBelt.h"

#include "MovableObject.h"
#include "NotEvenPlayer.h"
#include "Components/ArrowComponent.h"

// Sets default values
AConveyorBelt::AConveyorBelt()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComp->SetupAttachment(BoxComp);

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(BoxComp);
	OverlapBox->SetGenerateOverlapEvents(true);
    OverlapBox->SetRelativeLocation(FVector(0, 0, 70.f));

	MeshComp->SetRelativeLocation(FVector(0, 0, 55.f));
    MeshComp->SetRelativeScale3D(FVector(0.49f, 0.8f, 17.f));

	bIsInteractable = true;
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void AConveyorBelt::BeginPlay()
{
	Super::BeginPlay();
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AConveyorBelt::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AConveyorBelt::OnOverlapEnd);
}

// Called every frame
void AConveyorBelt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveObject(DeltaTime);
}

void AConveyorBelt::Interact(class ANotEvenPlayer* player)
{
	Super::Interact(player);

	OwnedObj.RemoveAll([](const TWeakObjectPtr<AMovableObject>& o)
	{
		return !o->IsValidLowLevel();
	});
	
	if (player->isGrab)
	{
		if (!OwnedObj.IsEmpty()) return;
		auto add = player->OwnedObj;
		add->AttachToComponent(OverlapBox,FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		player->DetachGrabObj(false);
		if (OwnedObj.Contains(add)) return;
		OwnedObj.Add(add);
	}
	else
	{
		if (OwnedObj.IsEmpty()) return;
		if (!OwnedObj[0]->IsValidLowLevel()) return;
		OwnedObj[0]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		player->AttachGrabObj(OwnedObj[0]);
	}
}

void AConveyorBelt::MoveObject(float DeltaTime)
{
	FVector offset = ArrowComp->GetForwardVector() * Speed * DeltaTime;

	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, AMovableObject::StaticClass());

	for (auto actor : OverlappingActors)
	{
		if (auto cast = Cast<AMovableObject>(actor))
		{
			if (OwnedObj.Contains(cast)) cast->AddActorWorldOffset(offset);
		}
	}
}

void AConveyorBelt::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OwnedObj.Contains(OtherActor) || OtherActor->GetOwner() != nullptr) return;
	
	if (auto cast = Cast<AMovableObject>(OtherActor))
	{
		OwnedObj.Add(cast);
	}
}

void AConveyorBelt::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor,
	class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto cast = Cast<AMovableObject>(OtherActor))
	{
		int32 index =OwnedObj.IndexOfByPredicate([cast](AMovableObject* obj)
		{
			return obj == cast;
		});

		if (index != INDEX_NONE)
		{
			OwnedObj.RemoveAt(index);
		}
	}
}

