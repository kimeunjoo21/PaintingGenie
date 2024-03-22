// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Interfaces/OnlineSessionInterface.h>
#include "NetGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API UNetGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;

	// 세션 만드는 함수

	// 세션 파괴하는 함수

	// 세션 검색 함수

	// 세션 참여 함수


public:
	// 이 변수통해 (세션 만들고, 세션 검색, 세션 참여)
	TSharedPtr<class IOnlineSubsystem, ESPMode::ThreadSafe> sessionInterface;

	// 세션 검색에 쓰이는 클래스
	TSharedPtr<class FOnlineSessionSearch> sessionSearch;

	// 세션 이름

	// 세션 검색이 완료되면 호출해야 하는 Delegate
};
