// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletColorPickerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Border.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/Button.h>



void UBulletColorPickerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//UButton 인클루드
	Btn_colorPickMenu-> OnClicked.AddDynamic(this, &UBulletColorPickerWidget::resizeWidget);
		
}

void UBulletColorPickerWidget::resizeWidget()
{	
	 //UBorder인클로드
	if(isColorPicker==true)
	{ 
		isColorPicker = false;
		boderwidget->SetVisibility(ESlateVisibility::Hidden);
		UE_LOG(LogTemp, Warning, TEXT("hidden picker"))

	}
	else
	{
		isColorPicker = true;
		boderwidget->SetVisibility(ESlateVisibility::Visible);
		UE_LOG(LogTemp, Warning, TEXT("visible picker"))
	}
}
