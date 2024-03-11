// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModeTitle.h"
#include "TitleWidget.h"

void AGameModeTitle::BeginPlay()
{
	Super::BeginPlay();

	//ConstructorHelpers::FClassFinder<UTitleWidget> tempTitleWidget(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_TitleWidget.WBP_TitleWidget_C'"));
	////TitleWidget �������Ʈ (WBP_TitleWidget) ��������
	//if (tempTitleWidget.Succeeded())
	//{
	//	titleWidgetSource = tempTitleWidget.Class;
	//}

	TitleWidgetUI = CreateWidget<UTitleWidget>(GetWorld(), titleWidgetSource);
	TitleWidgetUI->AddToViewport();

	// �÷��̾� ��Ʈ�ѷ� ���콺 Ŀ���� ȭ�鿡 ���̰� �ϱ�
	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}
