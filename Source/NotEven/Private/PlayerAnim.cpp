// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"

#include "NotEvenPlayer.h"

void UPlayerAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<ANotEvenPlayer>(TryGetPawnOwner());
}

void UPlayerAnim::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (Player)
	{
		FVector vel = Player->GetVelocity();
		Speed = FVector::DotProduct(vel, Player->GetActorForwardVector());
	}
}
