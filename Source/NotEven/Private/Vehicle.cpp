// Fill out your copyright notice in the Description page of Project Settings.


#include "Vehicle.h"

#include "NotEvenPlayer.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AVehicle::AVehicle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	boxComp -> SetupAttachment(RootComponent);
	boxComp ->SetBoxExtent(FVector(250.f, 150.f, 100.f));

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp -> SetupAttachment(boxComp);
	meshComp -> SetRelativeScale3D(FVector(5.f));
	meshComp -> SetRelativeLocationAndRotation(FVector(0, 0, -100.f), FRotator(0.f,-90.f,0.f));
	meshComp->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/KHB/Models/car_stationwagon.car_stationwagon'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
	}

	bReplicates = true;
}

// Called when the game starts or when spawned
void AVehicle::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AVehicle::OnBoxCompBeginOverlap);

	Direction = GetActorForwardVector();

	FRotator Rot = UKismetMathLibrary::MakeRotFromX(Direction);
	SetActorRotation(Rot);

	GetWorldTimerManager().SetTimer(DestroyTimer, this, &AVehicle::DestroyVehicle,10.f,false);
	
}

// Called every frame
void AVehicle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector newLoc = GetActorLocation() + Direction * Speed * DeltaTime;
	SetActorLocation(newLoc);
}

void AVehicle::OnBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//힘의 방향
	FVector impulseDir = this->GetActorForwardVector();
	impulseDir.Normalize();

	//힘의 세기
	float ImpulseStrength = 1500.f;
	FVector impulseResult = impulseDir * ImpulseStrength;
	
	ANotEvenPlayer* player = Cast<ANotEvenPlayer>(OtherActor);
	if (player)
	{
		player->DisableInput(Cast<APlayerController>(player->GetController()));
		player->GetMesh()->SetSimulatePhysics(true);
		player -> GetMesh()->AddImpulseToAllBodiesBelow(impulseResult,FName("body"),true,true);
		player->CallRestartPlayerDelay();
		player->DetachGrabObj(true);
	}
}


void AVehicle::DestroyVehicle()
{
	Destroy();
}
