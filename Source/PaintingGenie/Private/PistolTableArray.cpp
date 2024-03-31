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
	ptaArray.SetNum(10);

	//��ü �ʺ� = �ǹ��� ���� * �ǹ��� �ʺ� + (�ǹ��� ���� - 1) * �ǹ� ������ ����
	//float entireWidth = pitolTableActorChild.Num() * 10 + (pitolTableActorChild.Num() - 1) * tableTerm;

	// ���� ��ġ(y��) = ��ü �ʺ� * -0.5 + �ǹ��� �ʺ� * 0.5
	//float startLocation = -1.0f * entireWidth + 10 * 10.0f;

	// ��ġ ���� = �ǹ��� �ʺ� + �ǹ� ������ ����
	//float term = 10 + tableTerm;

	/*for (int32 i = 0; i < pitolTableActorChild.Num(); i++)
	{
		FString pistolTable = FString::Printf(TEXT("PistolTable_%d"), i + 1);
		UChildActorComponent* childPistolTable = CreateDefaultSubobject<UChildActorComponent>(FName(pistolTable));
		childPistolTable->SetupAttachment(RootComponent);

		FVector pisolTableLocation = FVector(0, startLocation + term * i, 0);
		childPistolTable->SetRelativeLocation(pisolTableLocation);
		pitolTableActorChild.Add(childPistolTable);
	}*/

	for (int32 i = 0; i < ptaArray.Num(); i++)
	{
		FString pistolTable = FString::Printf(TEXT("PistolTable_%d"), i + 1);
		UChildActorComponent* childPistolTable = CreateDefaultSubobject<UChildActorComponent>(FName(pistolTable));
		childPistolTable->SetupAttachment(RootComponent);

		FVector pisolTableLocation = FVector(0, 30*i, 0);
		childPistolTable->SetRelativeLocation(pisolTableLocation);
		pitolTableActorChild.Add(childPistolTable);
	}



	//APistolTable ��Ŭ���
	auto bp_pt = ConstructorHelpers::FClassFinder<AActor>(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_PistolTable.BP_PistolTable_C'"));
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
			UE_LOG(LogTemp, Warning, TEXT("set class suc"));

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("set class fail"));
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

