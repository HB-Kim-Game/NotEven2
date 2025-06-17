// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "LobbyPlayer.generated.h"

UCLASS()
class NOTEVEN_API ALobbyPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ALobbyPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(ReplicatedUsing=OnRep_PlayerIndex)
	int32 PlayerIndex;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
	void OnRep_PlayerIndex();

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostNetInit() override;

	UPROPERTY(EditDefaultsOnly,Category=PlayerColor)
	class UMaterialInstance* BlueColor;

	UPROPERTY(EditDefaultsOnly,Category=PlayerColor)
	class UMaterialInstance* RedColor;
	
	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere)
	class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* ParticleComp;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULobbyUI> LobbyUIClass;

	UPROPERTY()
	class ULobbyUI* LobbyUI;

	UFUNCTION()
	void CompleteCreateSession(FName SessionName, bool bWasSuccessful);

	FDelegateHandle CreateSessionDelegate;
};
