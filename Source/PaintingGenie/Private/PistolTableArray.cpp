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
	
	//UChildActorComponent ��Ŭ���
	pitolTableActorChild.SetNum(10);


	//��ü �ʺ� = �ǹ��� ���� * �ǹ��� �ʺ� + (�ǹ��� ���� - 1) * �ǹ� ������ ����
	float entireWidth = pitolTableActorChild.Num() * 10 + (pitolTableActorChild.Num() - 1) * tableTerm;

	// ���� ��ġ(y��) = ��ü �ʺ� * -0.5 + �ǹ��� �ʺ� * 0.5
	float startLocation = -0.5f * entireWidth + 10 * 0.5f;

	// ��ġ ���� = �ǹ��� �ʺ� + �ǹ� ������ ����
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

	//APistolTable ��Ŭ���
	auto bp_pt = ConstructorHelpers::FClassFinder<APistolTable>(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_PistolTable.BP_PistolTable'"));
	if (bp_pt.Succeeded())
	{
		pistolTableActor = bp_pt.Class;
	}

	if (pistolTableActor != nullptr)
	{
		for (int32 i = 0; i < pitolTableActorChild.Num(); i++)
		{
			// Child Actor ��� - childActorComponent�� ������ Ű���� ���͸� �Ҵ��Ѵ�.
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

