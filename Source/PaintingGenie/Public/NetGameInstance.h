// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"
#include <Interfaces/OnlineSessionInterface.h>
//파일이 없고 에러발생, 빌드문제로 주석처리함.
//#include "MoviePlayer.h"
#include "NetGameInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE_TwoParams(FSearchComplete, int32, FString);

UCLASS()
class PAINTINGGENIE_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	// 세션 만드는 함수
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString roomName, int32 maxPlayer);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션 파괴하는 함수
	UFUNCTION(BlueprintCallable)
	void DestroyMySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// 세션 검색 함수
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	// 세션 참여 함수
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	FString StringBase64Encode(FString str);
	FString StringBase64Decode(FString str);

public:
	// 이 변수통해 (세션 만들고, 세션 검색, 세션 참여)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;

	// 세션 검색에 쓰이는 클래스
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// 세션 이름
	FString mySessionName = TEXT("TestSession");

	// 세션 검색이 완료되면 호출해야 하는 Delegate
	FSearchComplete onSearchComplete; 


/*-------------------------- 로딩 화면 --------------------------*/
//public:
//	UFUNCTION(BlueprintCallable, meta= (DisplayName = "BeginLoadingScreen"), Category = "Loading Screen")
//	virtual void BeginLoadingScreen(const FString& MapName);
//	UFUNCTION(BlueprintCallable, meta = (DisplayName = "EndLoadingScreen"), Category = "Loading Screen")
//	virtual void EndLoadingScreen(UWorld* InLoadedWorld);
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
//	TSubclassOf<class UUserWidget> LoadingScreenWidget;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
//	bool useMovies;
//
//	// 사용할 Movies 경로 받을 변수
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
//	TArray<FString> StringPaths;

// Restore용 코드

};
