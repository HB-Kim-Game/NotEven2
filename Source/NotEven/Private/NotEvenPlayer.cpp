// Fill out your copyright notice in the Description page of Project Settings.


#include "NotEvenPlayer.h"

#include "CuttingBoard.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FoodIngredient.h"
#include "FoodSubmitTable.h"
#include "ImmovableObject.h"
#include "KitchenTable.h"
#include "MovableObject.h"
#include "NotEvenGameMode.h"
#include "Plate.h"
#include "SubmitFood.h"
#include "TrashBox.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/Core/Analytics/NetStatsUtils.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsShared/ImmediatePhysicsCore.h"


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

// Called every frame
void ANotEvenPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	AddMovementInput(Direction);
	
	Direction = FVector::ZeroVector;

//--------------------------------------------------------------------------------------//
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (auto tempGrabObj : AllActors)
	{
		if (tempGrabObj->GetActorNameOrLabel().Contains("Food"))
		{
			ObjActors.Add(tempGrabObj);
		}
	}
	
}

// Called to bind functionality to input
void ANotEvenPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (auto* input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// 이동
		input->BindAction(IA_PlayerMove,ETriggerEvent::Triggered,this,&ANotEvenPlayer::OnActionMove);
		// 대쉬
		input->BindAction(IA_PlayerDash,ETriggerEvent::Started,this,&ANotEvenPlayer::OnActionDash);
		// 잡기/놓기
		input->BindAction(IA_PlayerGrab,ETriggerEvent::Started,this,&ANotEvenPlayer::OnActionObjGrab);
		//던지기/ 다지기
		input ->BindAction(IA_PlayerChopAndThrow,ETriggerEvent::Started,this,&ANotEvenPlayer::OnActionObjChoppingAndThrowing);
	}
}

// 캐릭터 이동
void ANotEvenPlayer::OnActionMove(const FInputActionValue& value)
{
	FVector2D v = value.Get<FVector2D>();
	Direction.X = v.X;
	Direction.Y = v.Y;
}

// 캐릭터 대쉬
void ANotEvenPlayer::OnActionDash(const FInputActionValue& value)
{
	FVector forwordDir = this->GetActorRotation().Vector();
	LaunchCharacter(forwordDir*DashDistance,true,true);
}

// 캐릭터 잡기 입력
void ANotEvenPlayer::OnActionObjGrab(const FInputActionValue& value)
{
	TArray<FHitResult> hitResults;
	FVector boxExtent = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius()+10.f);
	boxExtent.Z = 300.f;

	if (isGrab)
	{
		// 잡을 수 없는 Obj Collision이면
		if (GetWorld()->SweepMultiByChannel(hitResults,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*ObjDistance,
	FQuat::Identity,ECC_GameTraceChannel2,FCollisionShape::MakeBox(boxExtent)))
		{
			for (auto tempGrabObj : hitResults)
			{
				// 만약에 테이블이면은
				if (auto table = Cast<AKitchenTable>(tempGrabObj.GetActor()))
				{
					// -> 테이블 위에 상호작용 하고싶다
					table ->Interact(this);
					return;
				}

				// 만약에 음식 제출테이블이면
				if (auto subtable = Cast<AFoodSubmitTable>(tempGrabObj.GetActor()))
				{
					// -> 제출테이블에 상호작용 하고싶다
					subtable ->Interact(this);
					return;
				}
				
				// 만약에 도마이면
				// -> 도마 위에 놓기를 하고 싶다
				if (auto cuttingBoard = Cast<ACuttingBoard>(tempGrabObj.GetActor()))
				{
					// 프라이팬, 도마 위에 이미 음식이 놓여져 있으면
					if (auto food = Cast<AFoodIngredient>(OwnedObj))
					{
						if (!food->GetIngredientPlaceData().PlacementRules.ContainsByPredicate([food](FIngredientPlaceRule& rule)
						{
							return rule.State == food->GetIngredientState() && rule.Requirement.Contains(EPlacementRequirement::CuttingBoard);
						}))
						{
							return;
						}
					}
					
					if (Cast<AFoodIngredient>(OwnedObj))
					{
						cuttingBoard ->Interact(this);
						return;
					}
				}

				// 3. 만약에 쓰레기통이면
				if (auto trashBox = Cast<ATrashBox>(tempGrabObj.GetActor()))
				{
					// 쓰레기통이랑 상호작용 하고싶다
					trashBox -> Interact(this);
					return;
				}
			}
		}
		DetachGrabObj();
		return;
	}
	
	hitResults.Empty();
	
	if (GetWorld()->SweepMultiByChannel(hitResults,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*ObjDistance,
		FQuat::Identity,ECC_GameTraceChannel2,FCollisionShape::MakeBox(boxExtent)))
	{
		for (auto tempGrabObj : hitResults)
		{
			if (auto* unGrabObj = Cast<AImmovableObject>(tempGrabObj.GetActor()))
			{
				unGrabObj->Interact(this);
				return;
			}
		}
	}
	
	hitResults.Empty();
	
	//잡을 수 있는 Obj Collision이면
	if (GetWorld()->SweepMultiByChannel(hitResults,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*ObjDistance,
		FQuat::Identity,ECC_GameTraceChannel1,FCollisionShape::MakeBox(boxExtent)))
	{
		for (auto tempGrabObj : hitResults)
		{
			AttachGrabObj(tempGrabObj.GetActor());

			return;
		}
	}
}

// 오브젝트 붙이기
void ANotEvenPlayer::AttachGrabObj(AActor* ObjActor)
{
	OwnedObj = Cast<AMovableObject>(ObjActor);
	OwnedObj->SetOwner(this);
	OwnedObj->Interact(this);
	isGrab = true;
}

// 오브젝트 떼기
void ANotEvenPlayer::DetachGrabObj()
{
	if (!OwnedObj)
		return;
	OwnedObj ->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	OwnedObj -> SetOwner(nullptr);
	OwnedObj->SetGrab(false);
	OwnedObj = nullptr;
	isGrab = false;
}

// 다지기 및 던지기
void ANotEvenPlayer::OnActionObjChoppingAndThrowing(const FInputActionValue& value)
{
	//만약에 잡기를 하고 있으면
	if (isGrab==true)
	{
		auto plateobj = Cast<APlate>(OwnedObj);
		auto foodobj = Cast<AFoodIngredient>(OwnedObj) ;
		FVector impulse = (GetActorForwardVector() * 1500.f + GetActorUpVector()*300.f) * 200.f;

		// 만약 접시이면
		if (plateobj)
		{
			//던지지 않는다
			return;
		}
		// 만약 음식이면
		if (foodobj)
		{
			//OwnedObj를 떼어내고
			DetachGrabObj();

			//힘을 가하고 싶다
			foodobj->BoxComp->AddImpulse(impulse);
		}
	}
	
	TArray<FHitResult> hitResults;
	FVector boxExtent = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius()+10.f);
	boxExtent.Z = 300.f;

	// 만약에 잡기를 안하고 있으면
	if (isGrab==false)
	{
		if (GetWorld()->SweepMultiByChannel(hitResults,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*ObjDistance,
		FQuat::Identity,ECC_GameTraceChannel2,FCollisionShape::MakeBox(boxExtent)))
		{
			for (auto tempGrabObj : hitResults)
			{
				if (auto* unGrabObj = Cast<ACuttingBoard>(tempGrabObj.GetActor()))
				{
					unGrabObj->Cutting(this);
					return;
				}
			}
		}
	}
}

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
