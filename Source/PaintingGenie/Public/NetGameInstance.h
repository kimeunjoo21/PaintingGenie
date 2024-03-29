// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Slate.h"
#include <Interfaces/OnlineSessionInterface.h>
//������ ���� �����߻�, ���幮���� �ּ�ó����.
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

	// ���� ����� �Լ�
	UFUNCTION(BlueprintCallable)
	void CreateMySession(FString roomName, int32 maxPlayer);
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	// ���� �ı��ϴ� �Լ�
	UFUNCTION(BlueprintCallable)
	void DestroyMySession();
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	// ���� �˻� �Լ�
	UFUNCTION(BlueprintCallable)
	void FindOtherSession();
	void OnFindSessionComplete(bool bWasSuccessful);

	// ���� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void JoinOtherSession(int32 idx);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type result);

	FString StringBase64Encode(FString str);
	FString StringBase64Decode(FString str);

public:
	// �� �������� (���� �����, ���� �˻�, ���� ����)
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> sessionInterface;

	// ���� �˻��� ���̴� Ŭ����
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// ���� �̸�
	FString mySessionName = TEXT("TestSession");

	// ���� �˻��� �Ϸ�Ǹ� ȣ���ؾ� �ϴ� Delegate
	FSearchComplete onSearchComplete; 


/*-------------------------- �ε� ȭ�� --------------------------*/
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
//	// ����� Movies ��� ���� ����
//	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loading Screen")
//	TArray<FString> StringPaths;

// Restore�� �ڵ�

};
