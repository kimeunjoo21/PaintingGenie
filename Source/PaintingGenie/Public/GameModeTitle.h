// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeTitle.generated.h"

/**
 * 
 */
UCLASS()
class PAINTINGGENIE_API AGameModeTitle : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UTitleWidget> titleWidgetSource;

	class UTitleWidget* TitleWidgetUI;
	
};
