// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */


UCLASS()
class PAINTINGGENIE_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()


public:

	// ��ư ���� ����
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_Play;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_Options;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_Quit;

	/*UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UButton* SinglePlay;

	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	UButton* MultiPlay;*/

	// WBP_TitleWidget �������Ʈ ���� �Ҵ��� ���� ���� ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTitleWidget> TitleWidget;

	// ���� �� ��Ʈ�� �ε��  TitleWidget(WBP_TitleWidget)�� ���� ���� UI�� ���� ����� ����
	UPROPERTY(EditAnywhere)
	class UTitleWidget* TitleWidgetUI;

public:
	// Title ȭ�� ���� ���� �Լ�
	void ShowTitleWidget();

	// Play ��ư�� ������ �� ���� �̵� �Լ�
	UFUNCTION()
	void StartSingleModeGame();

	// Quit ��ư�� ������ �� ���� ���� �Լ�
	UFUNCTION()
	void QuitGame();

protected:
	// BeginPlay �� ���� ����� �Լ�
	virtual void NativeConstruct() override;
	
};
