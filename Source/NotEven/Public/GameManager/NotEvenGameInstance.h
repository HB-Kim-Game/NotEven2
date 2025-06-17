// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "NotEvenGameInstance.generated.h"

// 세션 검색 끝났을 때 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSearchComplete);
// 세션 검색 시작시 호출될 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSearchBegin);

/**
 * 
 */
UCLASS()
class NOTEVEN_API UNotEvenGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UNotEvenGameInstance();
	
	virtual void Init() override;

	void CreateMySession(int32 playerCount);

	bool bIsJoinSession = false;
	
	IOnlineSessionPtr SessionInterface;

public: // Loading
	UPROPERTY()
	class ULoadingUI* LoadingUI;

	UPROPERTY()
	TSubclassOf<ULoadingUI> LoadingUIClass;

	void ShowLoadingScreen();
	void HideLoadingScreen();

public:
	// 방찾기
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	
	void FindOtherSessions();

	void OnFindSessionsComplete(bool bWasSuccessful);

	FSearchComplete OnSearchComplete;

	FSearchBegin OnBeginSearch;
	
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void OnDestroyAndFindSession(FName SessionName, bool bWasSuccessful);

public:
	void ExitRoom();

	void DestroySession();

	void QuitGame(FName sessionName, bool bWasSuccessful);

	UFUNCTION(Server, Reliable)
	void ServerRPC_ExitRoom();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_ExitRoom();

	void OnMyExitRoomComplete(FName sessionName, bool bWasSuccessful);

	void OnNetworkFailure(UWorld* World, UNetDriver *NetDriver, ENetworkFailure::Type FailureType, const FString &ErrorString = TEXT(""));
	
	// 아직 게임 방(세션)안에 있는지 여부
	bool IsInRoom();

private:
	UFUNCTION()
	void TravelLobby(FName sessionName, bool bWasSuccessful);

	FString mySessionName = "MyRoom";
};
