// Fill out your copyright notice in the Description page of Project Settings.


#include "P4AnimInstance.h"
//플레이어 이클루드
#include "PaintingGenieCharacter.h"

void UP4AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Player 가져오자
	//플레이어 모름 인클루드
	player = Cast<APaintingGenieCharacter>(TryGetPawnOwner());
}

void UP4AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

}
