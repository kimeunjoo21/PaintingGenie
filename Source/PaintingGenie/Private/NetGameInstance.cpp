// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Interfaces/OnlineSessionInterface.h>

void UNetGameInstance::Init()
{
	Super::Init();

	// 온라인 서브 시스템 가져오기
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();

	if (subsys)
	{
		// 세션 인터페이스 가져오기
		//sessionInterface = subsys -> GetSessionInterface();
	}

}
