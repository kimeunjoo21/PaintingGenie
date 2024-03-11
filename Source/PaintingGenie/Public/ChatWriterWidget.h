// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWriterWidget.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API UChatWriterWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// BeginPlay �� ���� ����� �Լ�
	virtual void NativeConstruct() override;
	
};
