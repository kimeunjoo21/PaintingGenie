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
	class UButton* Btn_SinglePlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_MultiPlay;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* Btn_GameInfo;
	
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

	// Single Play ��ư�� ������ �� ���� ������ �ٷ� �̵��ϴ� �Լ�
	UFUNCTION()
	void StartSingleModeGame();

	// Multi Play ��ư�� ������ �� LobbyMap���� �̵��ϴ� �Լ�
	UFUNCTION()
	void GoLobbyMap();

	// Quit ��ư�� ������ �� ���� ���� �Լ�
	UFUNCTION()
	void QuitGame();

protected:
	// BeginPlay �� ���� ����� �Լ�
	virtual void NativeConstruct() override;
	
};
