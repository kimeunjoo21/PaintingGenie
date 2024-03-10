// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "P4AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API UP4AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

//이벤트
public:
	//비긴플레이
	virtual void NativeInitializeAnimation() override;
	//틱
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


//선언부
public:
	//플레이어 선언
	UPROPERTY()
	class APaintingGenieCharacter* player;
	//총을 가지고 있는지 확인하자.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool hasPistol;


	
};
