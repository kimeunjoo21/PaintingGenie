// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolTable.h"
#include "../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "../../../../../Source/Runtime/Engine/Classes/Components/StaticMeshComponent.h"

// Sets default values
APistolTable::APistolTable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	SetRootComponent(boxComp);

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	meshComp->SetupAttachment(RootComponent);

	pistolActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Pistol Actor"));
	pistolActor->SetupAttachment(meshComp);
}

// Called when the game starts or when spawned
void APistolTable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolTable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

