// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/EditableTextBox.h>
#include "NetGameInstance.h"
#include "SessionSlotWidget.h"
#include <Components/ScrollBox.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/PlayerState.h>

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// game instance 가져오기
	gi = Cast<UNetGameInstance>(GetGameInstance());

	// 로그인 메인 화면 기능
	btn_MoveCreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickMoveCreateSession);
	btn_MoveSearchSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickMoveSearchSession);

	// text_PlayerCount를 초기값 설정
	text_PlayerCount->SetText(FText::AsNumber(slider_PlayerCount->GetValue()));

	// Slider의 값이 변경될 때 함수 등록
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);

	btn_CreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickCreateSession);

	// 세션 검색 화면 기능들
	btn_FindSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickFindSession);
	gi->onSearchComplete.BindUObject(this, &ULoginWidget::OnSearchComplete);

}

void ULoginWidget::OnClickMoveCreateSession()
{
	// widget switcher를 이용해서 1번째 Widget 활성화
	widgetSwitcher->SetActiveWidgetIndex(1);

	APlayerState* ps = UGameplayStatics::GetPlayerState(GetWorld(), 0);
}

void ULoginWidget::OnClickMoveSearchSession()
{
	// widget switcher를 이용해서 2번째 Canvas에 만들어둔 Widget 활성화
	widgetSwitcher->SetActiveWidgetIndex(2);
	// 바로 검색 시작
	OnClickFindSession();
}

void ULoginWidget::OnValueChanged(float Value)
{
	// player count 텍스트 갱신
	text_PlayerCount->SetText(FText::AsNumber(Value));
}

void ULoginWidget::OnClickCreateSession()
{
	// gi 생성 함수 
	gi->CreateMySession(edit_SessionName->GetText().ToString(), slider_PlayerCount->GetValue());
}

void ULoginWidget::OnClickFindSession()
{
	// scroll_RoomList의 자식들 지우면서 클릭시 마다 갱신(?)하기
	scroll_RoomList->ClearChildren();
	gi->FindOtherSession();

	text_FindSession->SetText(FText::FromString(TEXT("검색중...")));
	btn_FindSession->SetIsEnabled(false);
}

void ULoginWidget::OnSearchComplete(int32 idx, FString info)
{
	if (idx < 0)
	{
		text_FindSession->SetText(FText::FromString(TEXT("세션 검색")));
		btn_FindSession->SetIsEnabled(true);
	}
	else
	{
		// SessionInfoWidget 생성
		auto widget = CreateWidget<USessionSlotWidget>(GetWorld(), sessionInfoWidgetFactory);
		// Scroll_RoomList에 추가 
		scroll_RoomList->AddChild(widget);
		// 만들어진 sessionInfo에 데이터를 셋팅
		widget->SetInfo(idx, info);
	}
}
