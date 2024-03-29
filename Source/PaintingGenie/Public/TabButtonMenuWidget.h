// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TabButtonMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API UTabButtonMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
public:
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_GameInfo;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_MoveToStartLocation;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_GoTitleWidget;

	UPROPERTY()
	class APaintingGenieCharacter* PaintingGenieCharacter;

	UFUNCTION()
	void OnClickViewGameInfo();

	UFUNCTION()
	void OnClickMoveToStartLocation();
	
	UFUNCTION()
	void OnClickGoTitleWidget();

};
