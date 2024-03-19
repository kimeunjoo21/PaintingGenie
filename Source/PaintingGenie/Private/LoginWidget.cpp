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

	// game instance ������ ����
	

	// �α��� ���� ȭ�� ���
	btn_MoveCreateSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickMoveCreateSession);
	btn_MoveSearchSession->OnClicked.AddDynamic(this, &ULoginWidget::OnClickMoveSearchSession);

	// text_PlayerCount�� �ʱⰪ ����
	// text_PlayerCount->SetText(FText::AsNumber(slider_PlayerCount->GetValue()));
	// 
	// Slider�� ���� ����� �� �Լ� ���
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULoginWidget::OnValueChanged);
}

void ULoginWidget::OnClickMoveCreateSession()
{
	// widget switcher�� �̿��ؼ� 1��° Widget Ȱ��ȭ
	widgetSwitcher->SetActiveWidgetIndex(1);
}

void ULoginWidget::OnClickMoveSearchSession()
{

}

void ULoginWidget::OnValueChanged(float Value)
{
	// player count �ؽ�Ʈ ����
	text_PlayerCount->SetText(FText::AsNumber(Value));
}

void ULoginWidget::OnClickCreateSession()
{
	// ���� gi ���� �Լ� ���� ����
}
