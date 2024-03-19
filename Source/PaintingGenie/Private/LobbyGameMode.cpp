// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LoginWidget.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// login (lobby) widget ���� viewport�� ���̱�
	lobbyWidget = Cast<ULoginWidget>(CreateWidget(GetWorld(), lobbyWidgetFactory));
	lobbyWidget->AddToViewport();

	// ���콺 ���̰� �ϱ�
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
}
