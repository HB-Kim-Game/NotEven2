// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager/NotEvenGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Blueprint/UserWidget.h"
#include "LoadingUI.h"
#include "NotEvenPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

UNotEvenGameInstance::UNotEvenGameInstance()
{
	ConstructorHelpers::FClassFinder<ULoadingUI> loadingUIClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/KHB/UI/WBP_Loading.WBP_Loading_C'"));

	if (loadingUIClass.Succeeded())
	{
		LoadingUIClass = loadingUIClass.Class;
	}
}

void UNotEvenGameInstance::Init()
{
	Super::Init();

	if (auto subSys = IOnlineSubsystem::Get())
	{
		SessionInterface = subSys->GetSessionInterface();
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNotEvenGameInstance::TravelLobby);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNotEvenGameInstance::OnFindSessionsComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNotEvenGameInstance::OnJoinSessionComplete);

		GEngine->OnNetworkFailure().AddUObject(this, &UNotEvenGameInstance::OnNetworkFailure);
	}
}

void UNotEvenGameInstance::CreateMySession(int32 playerCount)
{
	FOnlineSessionSettings sessionSettings;
	// 1. Dedicated Server 접속여부
	sessionSettings.bIsDedicated = false;
	// 2. 로컬(랜선)매칭 할지 혹은 Steam 매칭을 사용할지 여부
	FName subSysName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *subSysName.ToString());
	sessionSettings.bIsLANMatch = subSysName == "NULL";
	// 3. 매칭이 온라인을 통해 노출될지 여부
	sessionSettings.bShouldAdvertise = true;
	// 4. 나의 온라인 상태(presence) 정보를 활용하게 해줄지 여부
	sessionSettings.bUsesPresence = true;
	// 5. 로비 사용 여부
	sessionSettings.bUseLobbiesIfAvailable = true;
	// 6. 게임진행중에 참여 허가할지 여부
	sessionSettings.bAllowJoinViaPresence = true;
	sessionSettings.bAllowJoinInProgress = true;
	// 7. 세션에 참여할 수 있는 최대 참여자 수
	sessionSettings.NumPublicConnections = playerCount;

	// 8. 커스텀 룸네임 설정
	sessionSettings.Set(FName("ROOM_NAME"), mySessionName,
	EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	// // 9. 호스트네임 설정
	// sessionSettings.Set(FName("HOST_NAME"), myName,            
	// EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	if (!GetWorld()->GetFirstLocalPlayerFromController()) return;

	ConnectedPlayers = 0;
	
	// NetID
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	SessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UNotEvenGameInstance::ShowLoadingScreen()
{
	if (!LoadingUI)
	{
		LoadingUI = CreateWidget<ULoadingUI>(this, LoadingUIClass);
	}
	LoadingUI->AddToViewport(100);
	GetWorld()->GetGameViewport()->AddViewportWidgetContent(LoadingUI->TakeWidget());
	
	LoadingUI->ShowStageInfo();
}

void UNotEvenGameInstance::HideLoadingScreen()
{
	if (LoadingUI)
	{
		GetWorld()->GetGameViewport()->RemoveViewportWidgetContent(LoadingUI->TakeWidget());
		LoadingUI->RemoveFromParent();
		LoadingUI = nullptr;
	}
}

void UNotEvenGameInstance::FindOtherSessions()
{
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNotEvenGameInstance::OnDestroyAndFindSession);
	SessionInterface->DestroySession(FName(*mySessionName));
}

void UNotEvenGameInstance::OnFindSessionsComplete(bool bWasSuccessful)
{
	// 찾기 실패시 아무것도 하지않음
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("FailedFindSession"));
		OnSearchComplete.Broadcast();
		return;
	}

	TArray<FOnlineSessionSearchResult> results;

	if (SessionSearch->SearchResults.Num() <= 0)
	{
		OnSearchComplete.Broadcast();
		return;
	}

	for (auto r : SessionSearch->SearchResults)
	{
		FString temp;
		if (r.Session.SessionSettings.Get(FName("ROOM_NAME"), temp))
		{
			if (temp.Equals(mySessionName))
			{
				results.Add(r);
			}
		}
	}

	UE_LOG(LogTemp,Warning, TEXT("Found Sessions : %d"), results.Num());

	auto result = results[FMath::RandRange(0, results.Num() - 1)];

	if (!result.IsValid())
	{
		OnSearchComplete.Broadcast();
		return;
	}

	result.Session.SessionSettings.bUsesPresence = true;
	result.Session.SessionSettings.bUseLobbiesIfAvailable = true;
	result.Session.SessionSettings.Get(FName("ROOM_NAME"), mySessionName);

	UE_LOG(LogTemp, Warning, TEXT("SuccessFindSession"));
	
	SessionInterface->JoinSession(0, FName(mySessionName), result);
}

void UNotEvenGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UE_LOG(LogTemp, Warning, TEXT("SuccessJoinSession"));
	
	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		auto pc = GetWorld()->GetFirstPlayerController();
		FString url;
		SessionInterface->GetResolvedConnectString(SessionName, url);
		bIsJoinSession = true;

		if (!url.IsEmpty())
		{
			pc->ClientTravel(url, TRAVEL_Absolute);
		}
	}
	else
	{
		OnSearchComplete.Broadcast();
	}
}

void UNotEvenGameInstance::OnDestroyAndFindSession(FName SessionName, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDestroyAndFindSession"));
	if (bWasSuccessful)
	{
		OnBeginSearch.Broadcast();

		// 찾을 조건들 설정
		SessionSearch = MakeShareable(new FOnlineSessionSearch());

		// 1. 존재여부를 검색 가능하게 해놓은 녀석만 찾자.
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

		// 2. Lan 사용 여부
		SessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == FName("NULL");

		// 3. 최대 검색 세션(방) 수
		SessionSearch->MaxSearchResults = 10;

		// 4. 세션 검색
		if (SessionInterface->FindSessions(0, SessionSearch.ToSharedRef()))
		{
			UE_LOG(LogTemp, Warning, TEXT("FindSessions() 요청 성공"));
		}
	}
	SessionInterface->OnDestroySessionCompleteDelegates.RemoveAll(this);
}

void UNotEvenGameInstance::ExitRoom()
{
	ServerRPC_ExitRoom();
}

void UNotEvenGameInstance::DestroySession()
{
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNotEvenGameInstance::QuitGame);
	SessionInterface->DestroySession(FName(*mySessionName));
	bIsJoinSession = false;
}

void UNotEvenGameInstance::QuitGame(FName sessionName, bool bWasSuccessful)
{
	UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UNotEvenGameInstance::OnMyExitRoomComplete(FName sessionName, bool bWasSuccessful)
{
	auto pc = GetWorld()->GetFirstPlayerController();
	FString url = TEXT("/Game/KHB/Maps/Lobby");
	pc->ClientTravel(url, TRAVEL_Absolute);
	SessionInterface->OnDestroySessionCompleteDelegates.RemoveAll(this);
}

void UNotEvenGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType,
	const FString& ErrorString)
{
	// 서버와의 연결이 끊기면 정상적으로 방을 나가고 싶다.
	if (FailureType == ENetworkFailure::Type::ConnectionLost)
	{
		MultiRPC_ExitRoom_Implementation();
	}
}

bool UNotEvenGameInstance::IsInRoom()
{
	FUniqueNetIdPtr uniqueId = GetWorld()->GetFirstLocalPlayerFromController()
	->GetUniqueNetIdForPlatformUser().GetUniqueNetId();

	return SessionInterface->IsPlayerInSession(FName(*mySessionName), *uniqueId);
}

void UNotEvenGameInstance::ServerRPC_ExitRoom_Implementation()
{
	MultiRPC_ExitRoom();
}

void UNotEvenGameInstance::MultiRPC_ExitRoom_Implementation()
{
	SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNotEvenGameInstance::OnMyExitRoomComplete);
	// 세션종료
	SessionInterface->DestroySession(FName(*mySessionName));
	bIsJoinSession = false;
}

void UNotEvenGameInstance::TravelLobby(FName sessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		bIsJoinSession = true;
		GetWorld()->ServerTravel("/Game/KHB/Maps/Lobby?listen?port=7777");
		FString url;
		SessionInterface->GetResolvedConnectString(sessionName, url);
	}
}
