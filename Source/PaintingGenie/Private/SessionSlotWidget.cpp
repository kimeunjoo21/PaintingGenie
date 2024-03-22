// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionSlotWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include "NetGameInstance.h"

void USessionSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	btn_JoinSession->OnClicked.AddDynamic(this, &USessionSlotWidget::OnClickJoinSession);
}

void USessionSlotWidget::OnClickJoinSession()
{
	auto gi = Cast<UNetGameInstance>(GetGameInstance());
	gi->JoinOtherSession(sessionIdx);
}

void USessionSlotWidget::SetInfo(int32 idx, FString info)
{
	// 몇 번재 세션인지 담아놓자
	sessionIdx = idx;

	// 화면에 보이는 정보 갱신
	text_SessionInfo->SetText(FText::FromString(info));
}
