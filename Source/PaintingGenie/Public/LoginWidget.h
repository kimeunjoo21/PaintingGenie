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
	// BeginPlay 와 같은 기능의 함수
	virtual void NativeConstruct() override;

public:

	// gameinstance 담는 변수
	UPROPERTY()
	class UNetGameInstance* gi;

	// 메인화면
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

	// 세션 생성 화면
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

	// 세션 검색 | 참여 화면
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

	// 세션 검색 완료되면 호출되는 함수
	UFUNCTION()
	void OnSearchComplete(int32 idx, FString info);

	// 위젯 스위처에서 메인 화면 (0번 캔버스)으로 가는 버튼 및 함수
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_CreateSession;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Exit_RoomList;

	UFUNCTION()
	void OnClickExit();
	
	// 메인화면 (0번 캔버스)에서 홈화면(TitleWidget)으로 가는 버튼 및 함수
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_GoTitleWidget;

	UFUNCTION() 
	void OnClickGoTitleWidget();
};