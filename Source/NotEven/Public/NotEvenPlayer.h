// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
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

	virtual void Destroyed();


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

	void OnActionMove(const FInputActionValue& value);
	void OnActionDash(const FInputActionValue& value);

	float DashDistance = 2000.f;

	void CallRestartPlayer();
	// //플레이어 죽음처리
	// void PlayerDie();
	// //리스폰 시 초기화
	// void ResetPlayer();
};
