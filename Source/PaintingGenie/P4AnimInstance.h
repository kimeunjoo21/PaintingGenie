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

//�̺�Ʈ
public:
	//����÷���
	virtual void NativeInitializeAnimation() override;
	//ƽ
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


//�����
public:
	//�÷��̾� ����
	UPROPERTY()
	class APaintingGenieCharacter* player;
	//���� ������ �ִ��� Ȯ������.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool hasPistol;


	
};
