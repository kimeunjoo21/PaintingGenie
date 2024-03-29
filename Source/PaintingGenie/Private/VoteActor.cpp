// Fill out your copyright notice in the Description page of Project Settings.


#include "VoteActor.h"
#include <Components/BoxComponent.h>
#include "GameFramework/PlayerController.h"
#include <../PaintingGenieCharacter.h>
#include <Kismet/GameplayStatics.h>


//#include "PaintingGenieCharacter.h"
//#include <../../../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h>

// Sets default values
AVoteActor::AVoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("box Comp"));
	SetRootComponent(boxComp);
	boxComp ->SetCollisionProfileName(TEXT("NoCollision"));
	
	smCube = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Cube"));
	smCube -> SetupAttachment(RootComponent);
	smCube -> SetCollisionProfileName(TEXT("NoCollision"));


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

