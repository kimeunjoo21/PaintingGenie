// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BulletColorPickerWidget.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API UBulletColorPickerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> bulletColorPicker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UImage*> bulletImageColor;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite)
// 	TArray<class FVector*> fcolorArray;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UBorder* boderwidget;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_colorPickMenu;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_apply;

	bool isColorPicker = true;
	
	//�÷���Ŀ ��
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USynth2DSlider* colorwheel;
	
	//�����̴� ����(���)
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Slider_value;

	//�����̴� ���Ľ�Ƽ(����)
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class USlider* Slider_opacity;





	virtual void NativeConstruct() override;
	UFUNCTION()
	void resizeWidget();
	void ChanegeColorWheel();
};
