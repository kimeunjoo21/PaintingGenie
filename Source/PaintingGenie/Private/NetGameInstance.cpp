// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Interfaces/OnlineSessionInterface.h>
#include <Online/OnlineSessionNames.h>
#include "MoviePlayer.h"

void UNetGameInstance::Init()
{
	Super::Init();

	// �¶��� ���� �ý��� ��������
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if (subsys)
	{
		sessionInterface = subsys->GetSessionInterface();
		sessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
		sessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnDestroySessionComplete);
		sessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);
		sessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnJoinSessionComplete);
	}

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNetGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UNetGameInstance::EndLoadingScreen);


}

void UNetGameInstance::CreateMySession(FString roomName, int32 maxPlayer)
{
	FOnlineSessionSettings sessionSettings;
	
	// true ���� �˻�
	sessionSettings.bShouldAdvertise = true;
	// steam ���� �ش� �ɼ��� true ������ ���� �� ����
	sessionSettings.bUseLobbiesIfAvailable = true;
	// ���� ���� ���ΰ�?
	sessionSettings.bAllowJoinViaPresence = true;

	// �ο� ��
	sessionSettings.NumPublicConnections = maxPlayer;

	// ���̸� �ѱ� ������ �ʵ��� base64 Encode
	roomName = StringBase64Encode(roomName);

	// Ŀ���� �ɼ�
	sessionSettings.Set(FName(TEXT("ROOM_NAME")), roomName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// ���� ���� ��û
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().
		GetUniqueNetId();

	int32 rand = FMath::RandRange(1, 100000);
	mySessionName += FString::Printf(TEXT("%d"), rand);
	sessionInterface->CreateSession(*netID, FName(mySessionName), sessionSettings);
}

void UNetGameInstance::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Success -- %s"), *SessionName.ToString());
		//   | ���������� Battle Map ���� �̵��ϱ� ���� ������ �ڵ� | �Ʒ��� ���ص� ����pc�� ���������� ������ ����
		GetWorld()->ServerTravel(TEXT("/Game/KEJ/Maps/LV_EJSoul_Slum?listen"));
		// GetWorld()->ServerTravel(TEXT("/Game/ThirdPerson/Maps/BattleMap?listen"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete Fail"));
	}
}

void UNetGameInstance::DestroyMySession()
{
	sessionInterface->DestroySession(FName(mySessionName));
}

void UNetGameInstance::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	// ���� �Ҹ��ߴ� �� log Ȯ���ϱ� ���� �뵵
	if (bWasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Success -- %s"), *SessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnDestroySessionComplete Fail"));
	}
}

void UNetGameInstance::FindOtherSession()
{
	sessionSearch = MakeShared<FOnlineSessionSearch>();
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	sessionSearch->MaxSearchResults = 10;

	// ���� �˻� ��û
	sessionInterface->FindSessions(0, sessionSearch.ToSharedRef());
}
	

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		auto results = sessionSearch->SearchResults;
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Success - count : %d"), results.Num());

		for (int32 i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult si = results[i];
			FString roomName;
			si.Session.SessionSettings.Get(FName(TEXT("ROOM_NAME")), roomName);
			UE_LOG(LogTemp, Warning, TEXT("%d name : %s, count : %d"), i, *roomName,
				si.Session.NumOpenPublicConnections);


			// ���� ���� ---> String���� 
			// ������ �ִ� �ο�
			int32 maxPlayer = si.Session.NumOpenPublicConnections;
			// ������ ���� �ο� (�ִ� �ο� - ���� �ο�)
			int32 currPlayer = maxPlayer - si.Session.NumOpenPublicConnections;

			roomName = StringBase64Decode(roomName);
			// ���̸� ( 5 / 10 )
			FString sessionInfo = FString::Printf(
				TEXT("%s ( %d / %d )"),
				*roomName, currPlayer, maxPlayer);
			onSearchComplete.ExecuteIfBound(i, sessionInfo);
		}

		// idx�� -1 �����ؼ� �˻� �Ϸ� �˷�����
		onSearchComplete.ExecuteIfBound(-1, TEXT(""));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnFindSessionComplete Fail"));
	}

}

void UNetGameInstance::JoinOtherSession(int32 idx)
{
	//ip �޾Ƽ� ���� �޾Ƶ��� �κ�
		//TArray<FOnlineSessionSearchResult>
	auto results = sessionSearch->SearchResults;
	if (sessionInterface == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("sessionInterface is null"));
	}
	if (results.Num() <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("results Zero"));
	}
	sessionInterface->JoinSession(0, FName(mySessionName), results[idx]);
}

void UNetGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result)
{
	if (result == EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Success : %d"), *SessionName.ToString());
		FString url;
		// �����ؾ� �ϴ� Listen ���� URL�� �޾� ����
		sessionInterface->GetResolvedConnectString(SessionName, url);
		UE_LOG(LogTemp, Warning, TEXT("Join session URL : %s"), *url);

		if (!url.IsEmpty())
		{
			// �ش� URL�� ��������
			APlayerController* pc = GetWorld()->GetFirstPlayerController();
			pc->ClientTravel(url, ETravelType::TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Fail : %d"), result);
	}
}

FString UNetGameInstance::StringBase64Encode(FString str)
{
	// Set �� �� : FString -> UTF8 (std::string) -> TArray<uint8> -> base64�� Encode
	std::string utf8String = TCHAR_TO_UTF8(*str);
	TArray<uint8> arrayData = TArray<uint8>((uint8*)(utf8String.c_str()), utf8String.length());
	return FBase64::Encode(arrayData);
}

FString UNetGameInstance::StringBase64Decode(FString str)
{
	// Get �� �� : base64 �� Decode -> TArray<uint8> -> TCHAR 
	TArray<uint8> arrayData;
	FBase64::Decode(str, arrayData);
	std::string utf8String((char*)(arrayData.GetData()), arrayData.Num());
	return UTF8_TO_TCHAR(utf8String.c_str());
}

void UNetGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		
		if (useMovies)
		{
			LoadingScreen.MoviePaths = StringPaths;
		}
		// LoadingScreen.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();
		UUserWidget* widget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenWidget);

		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UNetGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
}
