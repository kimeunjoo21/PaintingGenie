// Fill out your copyright notice in the Description page of Project Settings.


#include "TabButtonMenuWidget.h"
#include <Components/Button.h>
#include <Kismet/GameplayStatics.h>

void UTabButtonMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	btn_GameInfo->OnClicked.AddDynamic(this, &UTabButtonMenuWidget::OnClickViewGameInfo);
	btn_MoveToStartLocation->OnClicked.AddDynamic(this, &UTabButtonMenuWidget::OnClickMoveToStartLocation);
	btn_GoTitleWidget->OnClicked.AddDynamic(this, &UTabButtonMenuWidget::OnClickGoTitleWidget);
}

void UTabButtonMenuWidget::OnClickViewGameInfo()
{
	// 주말에 게임정보랑 키 정보 추가할 것임
}

void UTabButtonMenuWidget::OnClickMoveToStartLocation()
{
	// 캐릭터의 정보를 가져와 현재 위치를 시작 위치로 변경 (코드 작성중)
	// APlayerController * pc = GetController<APlayerController>();
	//APaintingGenieCharacter* PaintingGenieCharacter = GetWorld()->GetFirstPlayerController<APaintingGenieCharacter>();

	//if (PaintingGenieCharacter)
	//{
	//	//PaintingGenieCharacter->SetActorLocation(FVector(0, 0, 0));
	//}
	//return FVector::ZeroVector;
	
	//SetActorLocation(FVector(0,0,0));

}

void UTabButtonMenuWidget::OnClickGoTitleWidget()
{
	UGameplayStatics::OpenLevel(GetWorld(), "TitleMap");
}
