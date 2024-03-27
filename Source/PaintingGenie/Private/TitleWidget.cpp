// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleWidget.h"
#include <Components/Button.h>
#include <../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>

void UTitleWidget::NativeConstruct()
{

	Super::NativeConstruct();

	//ConstructorHelpers::FClassFinder<UTitleWidget> tempTitleWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_TitleWidget.WBP_TitleWidget_C'"));
	////TitleWidget 블루프린트 (WBP_TitleWidget) 가져오기
	//if (tempTitleWidget.Succeeded())
	//{
	//	TitleWidget = tempTitleWidget.Class;
	//}

	Btn_SinglePlay->OnClicked.AddDynamic(this, &UTitleWidget::StartSingleModeGame);
	Btn_MultiPlay->OnClicked.AddDynamic(this, &UTitleWidget::GoLobbyMap);
	Btn_Options->OnClicked.AddDynamic(this, &UTitleWidget::StartSingleModeGame);
	Btn_Quit->OnClicked.AddDynamic(this, &UTitleWidget::QuitGame);

}

void UTitleWidget::ShowTitleWidget()
{
	// Title 위젯을 뷰포트에 생성하기
	if (TitleWidget != nullptr)
	{
		TitleWidgetUI = CreateWidget<UTitleWidget>(GetWorld(), TitleWidget);
		
		if (TitleWidgetUI != nullptr)
		{
			TitleWidgetUI->AddToViewport();
		}
	}
}

void UTitleWidget::StartSingleModeGame()
{
	UGameplayStatics::OpenLevel(GetWorld(),"LV_EJSoul_Slum");
	//titleWidgetUI->SetVisibility(false);
}

void UTitleWidget::GoLobbyMap()
{
	UGameplayStatics::OpenLevel(GetWorld(), "LobbyMap");
}

void UTitleWidget::QuitGame()
{
	// 현재 실행중인 월드 오브젝트 가져오기
	UWorld* currentWorld = GetWorld();

	// 앱 종료
	UKismetSystemLibrary::QuitGame(currentWorld, currentWorld->GetFirstPlayerController(), EQuitPreference::Quit, false);
}
