// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PistolTableArray.generated.h"

UCLASS()
class PAINTINGGENIE_API APistolTableArray : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APistolTableArray();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "MySettings|Components")
	class USceneComponent* rootComp;

	UPROPERTY(EditAnywhere, Category = "MySettings|Components")
	TArray<class UChildActorComponent*> pitolTableActorChild;

	UPROPERTY(EditAnywhere, Category = "MySettings|Sources")
	float tableTerm = 10.0f;

	UPROPERTY(VisibleAnywhere, Category = "MySettings|Sources")
	TSubclassOf<class APistolTable> pistolTableActor;

};
