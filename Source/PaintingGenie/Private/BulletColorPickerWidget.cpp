// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletColorPickerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>



void UBulletColorPickerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UButton ��Ŭ���
	Btn_apply-> OnClicked.AddDynamic(this, &UBulletColorPickerWidget::resizeWidget);
		
}

void UBulletColorPickerWidget::resizeWidget()
{
	//UBorder��Ŭ�ε�
	boderwidget->SetVisibility(ESlateVisibility::Hidden);
}
