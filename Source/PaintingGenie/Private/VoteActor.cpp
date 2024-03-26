// Fill out your copyright notice in the Description page of Project Settings.


#include "VoteActor.h"

// Sets default values
AVoteActor::AVoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVoteActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

