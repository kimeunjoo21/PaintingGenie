// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolTableArray.h"
#include "../../../../../Source/Runtime/Engine/Classes/Components/ChildActorComponent.h"
#include "../../../../../Source/Runtime/Engine/Classes/Components/SceneComponent.h"
#include "PistolTable.h"

// Sets default values
APistolTableArray::APistolTableArray()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);
	
	//UChildActorComponent 인클루드
	pitolTableActorChild.SetNum(10);


	//전체 너비 = 건반의 개수 * 건반의 너비 + (건반의 개수 - 1) * 건반 사이의 간격
	float entireWidth = pitolTableActorChild.Num() * 10 + (pitolTableActorChild.Num() - 1) * tableTerm;

	// 시작 위치(y축) = 전체 너비 * -0.5 + 건반의 너비 * 0.5
	float startLocation = -0.5f * entireWidth + 10 * 0.5f;

	// 배치 간격 = 건반의 너비 + 건반 사이의 간격
	float term = 10 + tableTerm;

	for (int32 i = 0; i < pitolTableActorChild.Num(); i++)
	{
		FString pistolTable = FString::Printf(TEXT("Key_%d"), i + 1);
		UChildActorComponent* cPistolTable = CreateDefaultSubobject<UChildActorComponent>(FName(pistolTable));
		cPistolTable->SetupAttachment(RootComponent);
		
		FVector pisolTableLocation = FVector(0, startLocation + term * i, 0);
		cPistolTable->SetRelativeLocation(pisolTableLocation);
		pitolTableActorChild.Add(cPistolTable);
	}

	//APistolTable 인클루드
	auto bp_pt = ConstructorHelpers::FClassFinder<APistolTable>(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_PistolTable.BP_PistolTable'"));
	if (bp_pt.Succeeded())
	{
		pistolTableActor = bp_pt.Class;
	}

	if (pistolTableActor != nullptr)
	{
		for (int32 i = 0; i < pitolTableActorChild.Num(); i++)
		{
			// Child Actor 방식 - childActorComponent에 생성할 키보드 액터를 할당한다.
			pitolTableActorChild[i]->SetChildActorClass(pistolTableActor);
		}
	}
}


// Called when the game starts or when spawned
void APistolTableArray::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolTableArray::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

