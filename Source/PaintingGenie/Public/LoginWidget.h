// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginWidget.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API ULoginWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// BeginPlay �� ���� ����� �Լ�
	virtual void NativeConstruct() override;

public:

	// gameinstance ��� ����
	UPROPERTY()
	class UNetGameInstance* gi;

	// ����ȭ��
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveCreateSession;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveSearchSession;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* widgetSwitcher;

	UFUNCTION()
	void OnClickMoveCreateSession();
	UFUNCTION()
	void OnClickMoveSearchSession();

	// ���� ���� ȭ��
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* edit_SessionName;
	UPROPERTY(meta = (BindWidget))
	class USlider* slider_PlayerCount;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_PlayerCount;
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_CreateSession;

	UFUNCTION()
	void OnValueChanged(float Value);
	UFUNCTION()
	void OnClickCreateSession();

	// ���� �˻� | ���� ȭ��
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_FindSession;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* scroll_RoomList;
	//UPROPERTY(meta = (BindWidget))
	//class UButton* btn_Exit_RoomList;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* text_FindSession;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class USessionSlotWidget> sessionInfoWidgetFactory;

	UFUNCTION()
	void OnClickFindSession();

	// ���� �˻� �Ϸ�Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
	void OnSearchComplete(int32 idx, FString info);

	// ���� ����ó���� ���� ȭ�� (0�� ĵ����)���� ���� ��ư �� �Լ�
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_CreateSession;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_RoomList;

	UFUNCTION()
	void OnClickExit();
	
	// ����ȭ�� (0�� ĵ����)���� Ȩȭ��(TitleWidget)���� ���� ��ư �� �Լ�
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_GoTitleWidget;

	UFUNCTION() 
	void OnClickGoTitleWidget();
};