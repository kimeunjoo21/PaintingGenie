// Fill out your copyright notice in the Description page of Project Settings.


#include "P4AnimInstance.h"
//�÷��̾� ��Ŭ���
#include "PaintingGenieCharacter.h"

void UP4AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player ��������
	//�÷��̾� �� ��Ŭ���
	player = Cast<APaintingGenieCharacter>(TryGetPawnOwner());
}

void UP4AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}
