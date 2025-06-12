// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera.h"

#include "NotEvenPlayer.h"
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
	
	SetActorLocation(FVector(-4100.f, 0.f, 4800.f));
	
	// 회전 설정: Pitch -50도
	FRotator newRot = FRotator(-50.f, 0.f, 0.f); // Pitch, Yaw, Roll
	SetActorRotation(newRot);

	CameraComp->SetFieldOfView(35);

	CameraLocation=GetActorLocation();
}

// Called every frame
void ACamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto pc = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (!pc)
		return;

	AActor* currentTarget = pc->GetViewTarget();
	if (currentTarget != this)
	{
		pc->SetViewTargetWithBlend(this,0,VTBlend_Linear,0,false);
	}
	

	TArray<AActor*> Player;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ANotEvenPlayer::StaticClass(),Player);

	if (Player.Num() < 2)
		return;

	if (Player.Num()>=2)
	{
		FVector playerLoc1 = Player[0]->GetActorLocation();
		FVector playerLoc2 = Player[1]->GetActorLocation();
		
		// 두 플레이어 간의 거리
		CurrentDistance = FVector::Dist(playerLoc1, playerLoc2);
		
		
		// float Alpha = FMath::Clamp((CurrentDistance-MinDistance)/(MaxDistance-MinDistance), 0.f, 0.5f);
		// float newFov= FMath::Lerp(MinFov,MaxFov,Alpha);
		//
		// CameraComp->SetFieldOfView(newFov);

		// 두 플레이어 간의 거리 중간 벡터
		FVector midPoint = (playerLoc1+playerLoc2)/2;
		FVector targetLoc = CameraComp->GetComponentLocation();
		
		targetLoc.X = FMath::Clamp(midPoint.X,CameraLocation.X - MoveRange,CameraLocation.X + MoveRange);
		targetLoc.Y = FMath::Clamp(midPoint.Y,CameraLocation.Y - MoveRange,CameraLocation.Y + MoveRange);

		FVector newLoc = FMath::VInterpTo(GetActorLocation(),targetLoc,DeltaTime, 2.f);
		SetActorLocation(newLoc);

		
	}
}

