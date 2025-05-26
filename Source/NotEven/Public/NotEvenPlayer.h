// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "MovableObject.h"
#include "GameFramework/Character.h"
#include "NotEvenPlayer.generated.h"

UCLASS()
class NOTEVEN_API ANotEvenPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANotEvenPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 게임플레이 중 앸터가 소멸되었을 때 호출
	// virtual void Destroyed();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyControllerChanged() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Direction;

	
	UPROPERTY(EditAnywhere,Category=input)
	class UInputMappingContext* IMC_Player;
	
	UPROPERTY(EditAnywhere,Category=input)
	class UInputAction* IA_PlayerMove;

	UPROPERTY(EditAnywhere,Category=input)
	class UInputAction* IA_PlayerDash;

	UPROPERTY(EditAnywhere,category=input)
	class UInputAction* IA_PlayerGrab;
	
	UPROPERTY(EditAnywhere,category=input)
	class UInputAction* IA_PlayerChopAndThrow;

	// 이동 입력
	void OnActionMove(const FInputActionValue& value);
	// 대쉬 입력
	void OnActionDash(const FInputActionValue& value);
	// 잡기 입력
	void OnActionObjGrab(const FInputActionValue& value);
	// 던지기 입력
	void OnActionObjChoppingAndThrowing(const FInputActionValue& value);

	//대쉬 거리 변수
	float DashDistance = 2000.f;

	// 플레이어 캐릭터를 재시작할 게임 모드 클래스 호출
	void CallRestartPlayer();

	void CallRestartPlayerDelay();

	// 리스폰 딜레이 타이머
	FTimerHandle DelayTimer;
	
//--------------------------잡기,놓기-----------------------------
	
	// 잡기 상태 변수
	bool isGrab = false;

	UPROPERTY()
	AMovableObject* OwnedObj = nullptr;

	UPROPERTY(EditAnywhere,Category=Food)
	float ObjDistance = 300.f;

	UPROPERTY()
	TArray<AActor*> ObjActors;

	void AttachGrabObj(AActor* ObjActor);
	void DetachGrabObj(AActor* ObjActor);

//-----------------------던지기-----------------------
	UPROPERTY(EditAnywhere)
	class AFoodIngredient* foodObj;
	
	bool isThrown = false;
	// void ReleaseGradObj(AActor* ObjActor);
	//AActor* ObjActor
};
