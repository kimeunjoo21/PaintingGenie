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

	// 버튼 변수 선언
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

	// WBP_TitleWidget 블루프린트 파일 할당을 위한 변수 선언
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UTitleWidget> TitleWidget;

	// 현재 뷰 포트에 로드된  TitleWidget(WBP_TitleWidget)을 통해 만든 UI를 담을 저장용 변수
	UPROPERTY(EditAnywhere)
	class UTitleWidget* TitleWidgetUI;

public:
	// Title 화면 위젯 생성 함수
	void ShowTitleWidget();

	// Single Play 버튼을 눌렀을 때 메인 레벨로 바로 이동하는 함수
	UFUNCTION()
	void StartSingleModeGame();

	// Multi Play 버튼을 눌렀을 때 LobbyMap으로 이동하는 함수
	UFUNCTION()
	void GoLobbyMap();

	// Quit 버튼을 눌렀을 때 게임 종료 함수
	UFUNCTION()
	void QuitGame();

protected:
	// BeginPlay 와 같은 기능의 함수
	virtual void NativeConstruct() override;
	
};
