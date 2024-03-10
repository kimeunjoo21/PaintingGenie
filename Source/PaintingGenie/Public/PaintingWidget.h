// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PaintingWidget.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API UPaintingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// BeginPlay 와 같은 기능의 함수
	virtual void NativeConstruct() override;
	
};
