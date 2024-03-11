// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeTitle.h"
#include "TitleWidget.h"

void AGameModeTitle::BeginPlay()
{
	Super::BeginPlay();

	//ConstructorHelpers::FClassFinder<UTitleWidget> tempTitleWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_TitleWidget.WBP_TitleWidget_C'"));
	////TitleWidget 블루프린트 (WBP_TitleWidget) 가져오기
	//if (tempTitleWidget.Succeeded())
	//{
	//	titleWidgetSource = tempTitleWidget.Class;
	//}

	TitleWidgetUI = CreateWidget<UTitleWidget>(GetWorld(), titleWidgetSource);
	TitleWidgetUI->AddToViewport();

	// 플레이어 컨트롤러 마우스 커서를 화면에 보이게 하기
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
