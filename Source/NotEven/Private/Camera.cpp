// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"
#include "Components/SceneComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACamera::ACamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	SceneComp=CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
    if (SceneComp)
    {
    	SceneComp->SetupAttachment(RootComponent);
    }

	CameraComp=CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	if (CameraComp)
	{
		CameraComp->SetupAttachment(SceneComp);
	}
	
}

// Called when the game starts or when spawned
void ACamera::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorLocation(FVector(-910.f, 0.f, 1750.f));

	// 회전 설정: Pitch -50도
	FRotator newRot = FRotator(-65.f, 0.f, 0.f); // Pitch, Yaw, Roll
	SetActorRotation(newRot);

	auto pc = GetWorld()->GetFirstPlayerController();

	if (pc)
	{
		// 자기 자신을 ViewTarget으로 설정
		pc->SetViewTargetWithBlend(this);
	}
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

