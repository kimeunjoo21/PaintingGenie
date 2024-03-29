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
	// �ָ��� ���������� Ű ���� �߰��� ����
}

void UTabButtonMenuWidget::OnClickMoveToStartLocation()
{
	// ĳ������ ������ ������ ���� ��ġ�� ���� ��ġ�� ���� (�ڵ� �ۼ���)
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
