// Fill out your copyright notice in the Description page of Project Settings.


#include "NotEvenPlayer.h"


#include "CuttingBoard.h"
#include "InputMappingContext.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "FoodIngredient.h"
#include "ImmovableObject.h"
#include "KitchenSink.h"
#include "MovableObject.h"
#include "NotEvenGameMode.h"
#include "Plate.h"
#include "SubmitFood.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameManager/OrderManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "PlayerUI.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"
#include "GameManager/NotEvenGameInstance.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ANotEvenPlayer::ANotEvenPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructorHelpers::FObjectFinder<USkeletalMesh>tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/KHB/Models/Robot/robot.robot'"));
	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-90), FRotator(0,90,0));
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		
	}

	ConstructorHelpers::FObjectFinder<UInputMappingContext>tempIMC(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/LGJ/InPuts/IMC_Player.IMC_Player'"));
	if (tempIMC.Succeeded())
	{
		IMC_Player = tempIMC.Object;
	}
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	ConstructorHelpers::FObjectFinder<UNiagaraSystem> tempDashEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/KHB/Models/NS_DashDust.NS_DashDust'"));
	if (tempDashEffect.Succeeded())
	{
		DashEffect = tempDashEffect.Object;
		
	}

	ConstructorHelpers::FObjectFinder<UNiagaraSystem>tempTrailEffect(TEXT("/Script/Niagara.NiagaraSystem'/Game/LGJ/Niagara/NS_ObjectTrail.NS_ObjectTrail'"));
	if (tempTrailEffect.Succeeded())
	{
		ObjectTrail = tempTrailEffect.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave>tempDashSound(TEXT("/Script/Engine.SoundWave'/Game/LGJ/Sound/521996__kastenfrosch__whoosh-dash-2.521996__kastenfrosch__whoosh-dash-2'"));
	if (tempDashSound.Succeeded())
	{
		DashSound = tempDashSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave>tempGradSound(TEXT("/Script/Engine.SoundWave'/Game/LGJ/Sound/mouth-bubble-pop-om-fx-4-4-00-00.mouth-bubble-pop-om-fx-4-4-00-00'"));
	if (tempGradSound.Succeeded())
	{
		GrabSound = tempGradSound.Object;
	}

	ConstructorHelpers::FObjectFinder<USoundWave>tempThrowingSound(TEXT("/Script/Engine.SoundWave'/Game/LGJ/Sound/cartoon-slide-whistle-down-1-176647.cartoon-slide-whistle-down-1-176647'"));
	if (tempThrowingSound.Succeeded())
	{
		ThrowingSound = tempThrowingSound.Object;
	}
	bReplicates = true;
}

void ANotEvenPlayer::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	if (auto* pc = Cast<APlayerController>(Controller))
	{
		if ( auto* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(IMC_Player,0);
		}
	}
	
	
}

void ANotEvenPlayer::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ANotEvenPlayer,PlayerIndex);
	
}


// Called when the game starts or when spawned
void ANotEvenPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (auto pc = GetWorld()->GetFirstPlayerController())
	{
		pc->SetInputMode(FInputModeGameOnly());
		pc->SetShowMouseCursor(false);
		
		if (!pc->IsLocalController()) return;
		
		if (auto ui = Cast<AOrderManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AOrderManager::StaticClass())))
		{
			ui -> InitWidget();
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ANotEvenPlayer::ServerRPC_NotifyReady);
	}
}

void ANotEvenPlayer::ServerRPC_NotifyReady_Implementation()
{
	if (auto gm = Cast<ANotEvenGameMode>(GetWorld()->GetAuthGameMode()))
	{
		gm->NotifyClientLoaded();
	}
}

void ANotEvenPlayer::NetMulticast_GameStart_Implementation()
{
	if (!IsLocallyControlled()) return;
	if (auto order = Cast<AOrderManager>(UGameplayStatics::GetActorOfClass(GetWorld(),AOrderManager::StaticClass())))
	{
		Cast<UNotEvenGameInstance>(GetGameInstance())->HideLoadingScreen();
		order->PlayerUI->GameStart();
	}
}

void ANotEvenPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	if (HasAuthority())
	{
		PlayerIndex = GetWorld()->GetGameState()->PlayerArray.IndexOfByKey(GetPlayerState());
		OnRep_PlayerIndex();
		UE_LOG(LogTemp,Display,TEXT("Player Index is %d"),PlayerIndex);
	}
}

void ANotEvenPlayer::PostNetInit()
{
	Super::PostNetInit();

	OnRep_PlayerIndex();
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

void ANotEvenPlayer::OnRep_PlayerIndex()
{
	GetMesh()->SetMaterial(0,PlayerIndex > 0 ? RedColor : BlueColor);
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
	ServerRPC_Dash();
}

// 캐릭터 잡기 입력
void ANotEvenPlayer::OnActionObjGrab(const FInputActionValue& value)
{
	ServerRPC_Grab();
}

// 오브젝트 붙이기
void ANotEvenPlayer::AttachGrabObj(AActor* ObjActor)
{
	Server_AttachGrabObj(ObjActor);	
}

void ANotEvenPlayer::Server_AttachGrabObj_Implementation(AActor* ObjActor)
{
	isGrab = true;
	OwnedObj = Cast<AMovableObject>(ObjActor);
	NetMulticast_AttachGrabObj(OwnedObj, true);
}

void ANotEvenPlayer::NetMulticast_AttachGrabObj_Implementation(class AMovableObject* obj, bool bIsGrab)
{
	if (!obj) return;
	isGrab = bIsGrab;
	OwnedObj = obj;
	OwnedObj->SetOwner(this);

	UE_LOG(LogTemp, Warning, TEXT("%s"), *OwnedObj->GetName());
	obj->SetGrab(true);
	obj->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GrabPoint"));
	
	if (GrabSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), GrabSound);
	}
}

// 오브젝트 떼기
void ANotEvenPlayer::DetachGrabObj(bool bIsSimulatedPhysics)
{
	Server_DetachGrabObj(bIsSimulatedPhysics);
}

void ANotEvenPlayer::NetMulticast_DetachGrabObj_Implementation(AMovableObject* detach, bool bIsSimulatedPhysics)
{
	if (!detach) return;
	isGrab = false;
	OwnedObj = nullptr;
	detach->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	detach->SetOwner(nullptr);
	detach->SetGrab(false);
	detach->BoxComp->SetSimulatePhysics(bIsSimulatedPhysics);
	detach->BoxComp->SetCollisionEnabled(bIsSimulatedPhysics ? ECollisionEnabled::Type::QueryAndPhysics : ECollisionEnabled::Type::NoCollision);
	
	if (GrabSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), GrabSound);
	}
}

void ANotEvenPlayer::Server_DetachGrabObj_Implementation(bool bIsSimulatedPhysics)
{
	isGrab = false;
	AMovableObject* temp = OwnedObj;
	OwnedObj = nullptr;
	NetMulticast_DetachGrabObj(temp, bIsSimulatedPhysics);
}

// 다지기 및 던지기
void ANotEvenPlayer::OnActionObjChoppingAndThrowing(const FInputActionValue& value)
{
	ServerRPC_ChopAndThrow();
}

void ANotEvenPlayer::ServerRPC_ChopAndThrow_Implementation()
{
	//만약에 잡기를 하고 있으면
	if (isGrab==true)
	{
		auto plateobj = Cast<APlate>(OwnedObj);
		auto foodobj = Cast<AFoodIngredient>(OwnedObj);
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
			//힘을 가하고 싶다
			AFoodIngredient* temp = foodobj;
			OwnedObj = nullptr;
			isGrab = false;
			NetMulticast_Throwing(temp);
		}
	}
	
	TArray<FHitResult> hitResults;
	FVector boxExtent = FVector(GetCapsuleComponent()->GetScaledCapsuleRadius());
	boxExtent.Z = 300.f;

	// 만약에 잡기를 안하고 있으면
	if (isGrab==false)
	{
		if (GetWorld()->SweepMultiByChannel(hitResults,GetActorLocation(),GetActorLocation()+GetActorForwardVector()*ObjDistance,
		FQuat::Identity,ECC_GameTraceChannel2,FCollisionShape::MakeBox(boxExtent)))
		{
			for (auto tempGrabObj : hitResults)
			{
				// 도마
				if (auto* unGrabObj = Cast<ACuttingBoard>(tempGrabObj.GetActor()))
				{
				
					unGrabObj->Cutting(this);
					
					return;
				}

				// 싱크대
				if (auto* unGrabObj = Cast<AKitchenSink>(tempGrabObj.GetActor()))
				{
					unGrabObj->Washing(this);
					return;
				}
			}
		}
	}
}

void ANotEvenPlayer::ServerRPC_Dash_Implementation()
{
	NetMulticast_Dash();
}

void ANotEvenPlayer::NetMulticast_Dash_Implementation()
{
	if (DashEffect)
	{
		FVector effectLocation = GetActorLocation();
		effectLocation -= GetActorForwardVector() * 30.f;
		auto effect = UNiagaraFunctionLibrary::SpawnSystemAttached(DashEffect, GetMesh(), FName(), effectLocation, GetActorRotation(),
			EAttachLocation::Type::KeepWorldPosition, true);
	}
	if (DashSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), DashSound);
	}
	FVector forwordDir = this->GetActorRotation().Vector();
	LaunchCharacter(forwordDir*DashDistance,true,true);
}

void ANotEvenPlayer::NetMulticast_Throwing_Implementation(class AFoodIngredient* foodobj)
{
	if (!foodobj) return;

	foodobj->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	foodobj->SetOwner(nullptr);
	foodobj->SetGrab(false);
	foodobj->BoxComp->SetSimulatePhysics(true);
	foodobj->BoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	if (ObjectTrail)
	{
		FVector effectLocation = foodobj->GetActorLocation();
		effectLocation -= GetActorForwardVector() * 30.f;
		auto effect = UNiagaraFunctionLibrary::SpawnSystemAttached(ObjectTrail, foodobj->MeshComp, FName(), effectLocation, GetActorRotation(),
		EAttachLocation::Type::KeepWorldPosition, true);
	}
	if (ThrowingSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), ThrowingSound);
	}
	FVector impulse = (GetActorForwardVector() * 1500.f + GetActorUpVector()*300.f) * 200.f;
	
	foodobj->BoxComp->AddImpulse(impulse);
}

void ANotEvenPlayer::OnGrab()
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
				if (auto immovableObj = Cast<AImmovableObject>(tempGrabObj.GetActor()))
				{
					if (immovableObj->bIsInteractable)
					{
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
						
						immovableObj->Interact(this);
						return;
					}
				}
			}
		}
		DetachGrabObj(true);
		return;
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
}

void ANotEvenPlayer::ServerRPC_Grab_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnGrab Server"));
	OnGrab();
}

void ANotEvenPlayer::CallRestartPlayer()
{
	if (HasAuthority()) ServerRPC_RestartPlayer();
}

void ANotEvenPlayer::ServerRPC_RestartPlayer_Implementation()
{
	//월드와 월드의 게임 모드가 RestartPlayer 함수를 호출하도록 함
	if (UWorld* World = GetWorld())
	{
		if (ANotEvenGameMode* gm = Cast<ANotEvenGameMode>(World->GetAuthGameMode()))
		{
			gm->RestartPlayer(GetController());
		}
	}
}

void ANotEvenPlayer::CallRestartPlayerDelay()
{
	GetWorld()->GetTimerManager().SetTimer(DelayTimer,this,&ANotEvenPlayer::CallRestartPlayer,5.f,false);
}
