// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include <OnlineSubsystem.h>
#include <OnlineSessionSettings.h>
#include <Interfaces/OnlineSessionInterface.h>

void UNetGameInstance::Init()
{
	Super::Init();

	// �¶��� ���� �ý��� ��������
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();

	if (subsys)
	{
		// ���� �������̽� ��������
		//sessionInterface = subsys -> GetSessionInterface();
	}

}
