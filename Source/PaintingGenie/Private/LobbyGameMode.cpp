// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LoginWidget.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	// login (lobby) widget 만들어서 viewport에 붙이기
	lobbyWidget = Cast<ULoginWidget>(CreateWidget(GetWorld(), lobbyWidgetFactory));
	lobbyWidget->AddToViewport();

	// 마우스 보이게 하기
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
}
