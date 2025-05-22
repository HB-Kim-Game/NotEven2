// Fill out your copyright notice in the Description page of Project Settings.


#include "NotEvenPlayer.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NotEvenGameMode.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

// Sets default values
ANotEvenPlayer::ANotEvenPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/LGJ/Assets/Customizable_Stickman_Hypercasual/BananaMan.BananaMan'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,-90,0));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext>tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/LGJ/InPuts/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
	{
		IMC_Player = tempIMC.Object;
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	PhysicsHandleComp = CreateDefaultSubobject<UPhysicsHandleComponent>(TEXT("PhysicsHandleComp"));
	
}

void ANotEvenPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (auto* pc = Cast<APlayerController>(Controller))
	{
		auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());

		subsystem->RemoveMappingContext(IMC_Player);
		subsystem->AddMappingContext(IMC_Player,0);
	}
}


// Called when the game starts or when spawned
void ANotEvenPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

// void ANotEvenPlayer::Destroyed()
// {
// 	Super::Destroyed();
//
// 	// 게임 모드에서 OnPlayerDied 이벤트에 바인딩한 예
// 	if (UWorld* World = GetWorld())
// 	{
// 		if (auto* gm = Cast<ANotEvenGameMode>(World->GetAuthGameMode()))
// 		{
// 			gm->GetOnPlayerDied().Broadcast(this);
// 		}
// 	}
// }

void ANotEvenPlayer::CallRestartPlayer()
{
	// 폰 컨트롤러에 대한 래퍼런스 구하기
	APlayerController* controllerRef = GetWorld()->GetFirstPlayerController();
	
	//월드와 월드의 게임 모드가 RestartPlayer 함수를 호출하도록 함
	if (UWorld* World = GetWorld())
	{
		if (ANotEvenGameMode* gm = Cast<ANotEvenGameMode>(World->GetAuthGameMode()))
		{
			gm-> RestartPlayer(controllerRef);
		}
	}
}

void ANotEvenPlayer::CallRestartPlayerDelay()
{
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,this,&ANotEvenPlayer::CallRestartPlayer,5.f,false);
}


// Called every frame
void ANotEvenPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform t =  FTransform(GetControlRotation());
	AddMovementInput(Direction);

	Direction = FVector::ZeroVector;

}

// Called to bind functionality to input
void ANotEvenPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		input->BindAction(IA_PlayerMove,ETriggerEvent::Triggered,this,&ANotEvenPlayer::OnActionMove);
		input->BindAction(IA_PlayerDash,ETriggerEvent::Started,this,&ANotEvenPlayer::OnActionDash);
	}
}

void ANotEvenPlayer::OnActionMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
	
}

void ANotEvenPlayer::OnActionDash(const FInputActionValue& value)
{
	FVector forwordDir = this->GetActorRotation().Vector();
	LaunchCharacter(forwordDir*DashDistance,true,true);
}

// void ANotEvenPlayer::PlayerDie()
// {
// 	APlayerController* pc = Cast<APlayerController>(GetController());
// 	if (pc)
// 	{
// 		pc->UnPossess();
// 	}
//
// 	//일정시간 후 파괴 및 리스폰
// 	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
// 	{
// 		ANotEvenGameMode* gm = Cast<ANotEvenGameMode>(UGameplayStatics::GetGameMode(this));
// 		if (gm)
// 		{
// 			gm->RequestRespawn(GetController());
// 		}
// 		Destroy();
// 	});
// }
//
// void ANotEvenPlayer::ResetPlayer()
// {
// 	
// }
