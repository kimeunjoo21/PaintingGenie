// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"
#include <Components/Button.h>
#include <Components/WidgetSwitcher.h>
#include <Components/Slider.h>
#include <Components/TextBlock.h>
#include <Components/EditableTextBox.h>

void ULoginWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// game instance 가져올 예정
	

	// 로그인 메인 화면 기능
	btn_MoveCreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickMoveCreateSession);
	btn_MoveSearchSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickMoveSearchSession);

	// text_PlayerCount를 초기값 설정
	// text_PlayerCount->SetText(FText::AsNumber(slider_PlayerCount->GetValue()));
	// 
	// Slider의 값이 변경될 때 함수 등록
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);
}

void ULoginWidget::OnClickMoveCreateSession()
{
	// widget switcher를 이용해서 1번째 Widget 활성화
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULoginWidget::OnClickMoveSearchSession()
{

}

void ULoginWidget::OnValueChanged(float Value)
{
	// player count 텍스트 갱신
	text_PlayerCount->SetText(FText::AsNumber(Value));
}

void ULoginWidget::OnClickCreateSession()
{
	// 추후 gi 생성 함수 구현 예정
}
