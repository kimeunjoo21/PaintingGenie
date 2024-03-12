// Copyright Epic Games, Inc. All Rights Reserved.

#include "PaintingGenieCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

//게임 플레이 스태틱 인클루드
#include "Kismet/GameplayStatics.h"
//애님 인스턴스 인클루드
#include "P4AnimInstance.h"
//씬 컴포넌트
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/DecalComponent.h>
//머티리얼 인클루드
//#include "Runtime/Engine/Classes/Materials/Material.h"


DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APaintingGenieCharacter

APaintingGenieCharacter::APaintingGenieCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// 총이 붙을 컴포넌트 만들자
	compGun = CreateDefaultSubobject<USceneComponent>(TEXT("GUN"));
	compGun->SetupAttachment(GetMesh(), FName(TEXT("GunPosition")));
	compGun->SetRelativeLocation(FVector(-7.144f, 3.68f, 4.136f));
	compGun->SetRelativeRotation(FRotator(3.4f, 75.699f, 6.6424f));

	//총알 색깔을 정하자
	SetBulletColor();





}

void APaintingGenieCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// AnimInstance 가져오자
	anim = Cast<UP4AnimInstance>(GetMesh()->GetAnimInstance());

	// 1. 바닥에 깔려있는 Pistol 을 찾자.
	//액터를 배역로 하고 변수에 저장.
	TArray<AActor*> allActor;
	//게임 플레이 스태틱을 인클루드
	//월드에 배치된 모든 클래중에서 피스톨이라는 이름을 찾아서 배열에 넣자.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActor);
	for (int32 i = 0; i < allActor.Num(); i++)
	{
		//액터의 라벨이 피스톨이라면
		if (allActor[i]->GetActorLabel().Contains(TEXT("Pistol")))
		{
			allPistol.Add(allActor[i]);
			//UE_LOG(LogTemp, Warning, TEXT("allpistrol"));

		}
	}

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void APaintingGenieCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::Look);

		// 헤더에서 선언한 TakePistolAction을 바인드하자.
		//총잡기
		EnhancedInputComponent->BindAction(TakePistolAction, ETriggerEvent::Started, this, &APaintingGenieCharacter::TakePistol);

		// 헤더에서 선언한 FireAction을 바인드하자.
		// 총 쏘기 단발
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APaintingGenieCharacter::Fire);
		//총쏘기 연발
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::Fire);
		//불릿색깔 이전 바꾸기
		EnhancedInputComponent->BindAction(afterBullet, ETriggerEvent::Started, this, &APaintingGenieCharacter::afterBulletColor);
		//UE_LOG(LogTemp, Warning, TEXT("afterBulletColor"));
		//불릿색깔 이전 바꾸기
		EnhancedInputComponent->BindAction(beforeBullet, ETriggerEvent::Started, this, &APaintingGenieCharacter::beforeBulletColor);
		//UE_LOG(LogTemp, Warning, TEXT("beforeBulletColor"));
		
		//불렛의 크기 바꾸기
		EnhancedInputComponent->BindAction(bulletScaleUpValue, ETriggerEvent::Started, this, &APaintingGenieCharacter::bulletScaleUp);
		EnhancedInputComponent->BindAction(bulletScaleUpValue, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::bulletScaleUp);
		UE_LOG(LogTemp, Warning, TEXT("bulletScaleUpValue"));
		
		//불렛의 크기 바꾸기
		EnhancedInputComponent->BindAction(bulletScaleDownValue, ETriggerEvent::Started, this, &APaintingGenieCharacter::bulletScaleDown);
		EnhancedInputComponent->BindAction(bulletScaleDownValue, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::bulletScaleDown);
		UE_LOG(LogTemp, Warning, TEXT("bulletScaleDownValue"));



	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void APaintingGenieCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APaintingGenieCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APaintingGenieCharacter::TakePistol()
{
	// 만약에 총을 들고 있다면
	if (closestPistol)
	{
		//총을 손에서 때자
		DetachPistol();
		//피스톨 상태를 null로
		closestPistol = nullptr;
		//리턴
		return;
	}

	
	// 총을 들고 있지 않다면
	//총과의 거리를 최대값까지 넣는다.
	float closestDist = std::numeric_limits<float>::max();
	closestPistol = nullptr;

	//배열 i는 올피스톨
	for (int32 i = 0; i < allPistol.Num(); i++)
	{
		// 1. 모든 Pistold 에서 나와의 거리를 구하자.
		//현재 나의 위치와 N번째 모든 피스톨의 위치를 변수에 저장하자.
		float dist = FVector::	Distance(GetActorLocation(), allPistol[i]->GetActorLocation());

		// 내가 집을 수 있는 범위에 있니?
		if (dist > takeGunDist) continue;

		// closestDist 보다 dist 작니?
		if (closestDist > dist)
		{
			// closestDist 를 dist 로 갱신
			closestDist = dist;
			// closestPistol 를 allPistol[i] 로 갱신
			closestPistol = allPistol[i];
		}
	}

	AttachPistol();


}

void APaintingGenieCharacter::AttachPistol()
{
	// 가까운 총이 없으면 함수를 나가자
	if (closestPistol == nullptr) return;
	UE_LOG(LogTemp, Warning, TEXT("find pistol"));

	// 물리적인 현상 Off 시켜주자
	auto compMesh = closestPistol->GetComponentByClass<UStaticMeshComponent>();
	compMesh->SetSimulatePhysics(false);

	// 가장 가까운 총을  Mesh -> GunPosition 소켓에 붙이자.
	closestPistol->AttachToComponent(compGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// animInstance 에 있는 hasPistol 을 true
	anim = Cast<UP4AnimInstance>(GetMesh()->GetAnimInstance());
	anim->hasPistol = true;

	//총을 붙이면 화면을 확대 해주자.
	// bOrientRotaionToMovement 꺼주자
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// RotationYaw 켜주자
	bUseControllerRotationYaw = true;
	// SprintArm 위치 바꿔주자
	CameraBoom->TargetArmLength = 100;
	CameraBoom->SetRelativeLocation(FVector(-4.33f, 33.8f, 70));


}

void APaintingGenieCharacter::DetachPistol()
{
	// 물리적인 현상 On 시켜주자
	auto compMesh = closestPistol->GetComponentByClass<UStaticMeshComponent>();
	compMesh->SetSimulatePhysics(true);
	// closestPistol 을 compGun 떨어져 나가자
	closestPistol->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	// animInstance 에 있는 hasPistol 을 false
	anim = Cast<UP4AnimInstance>(GetMesh()->GetAnimInstance());
	anim->hasPistol = false;

	//카메라 언줌
	// bOrientRotaionToMovement 켜주자
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// RotationYaw 꺼주자
	bUseControllerRotationYaw = false;
	// SprintArm 위치 바꿔주자
	CameraBoom->TargetArmLength = 400;
	CameraBoom->SetRelativeLocation(FVector::ZeroVector);

}

void APaintingGenieCharacter::Fire()
{
	// 총을 들고 있지 않으면 함수를 나가자
	// 총알이 0개면 함수를 나가자
	// 재장전 중에는 함수를 나가자
	//if (closestPistol == nullptr || currBulletCnt <= 0 || isReloading) return;
	
	//피스톨이 없으면 리턴
	if (closestPistol == nullptr)return;
	{

	FHitResult hitInfo;
	FVector startPos = FollowCamera->GetComponentLocation();
	FVector endPos = startPos + FollowCamera->GetForwardVector() * 100000;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);
	
	
	if (isHit)
	{
		//스폰 데칼의 어테치의 매개변수
		/*UDecalComponent* UGameplayStatics::SpawnDecalAttached(class UMaterialInterface* DecalMaterial, FVector DecalSize, class USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, float LifeSpan)*/
		
		//UGameplayStatics::SpawnDecalAttached(pistolPaint,FVector(10) params,);
		//스폰액터 (위치, 타입, 크기, 위치, 방향, 시간(0=무제한))
		//->SetSortOrder(order);
		//->SetSortOrder(order); 셋 소트오더를 통해서 레이어를 최상위로 올립니다.
		// order++; 오더를 누적합니다.  

		//bsc = decalsize FVector(50)
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), pistolpaintArray[pbn], FVector(BSC), hitInfo.ImpactPoint, FRotator::ZeroRotator, 0)->SetSortOrder(order);
		order++;
		
		//UE_LOG(LogTemp, Warning, TEXT("Spawn Decal"));
		
		//충돌시 폭발 효과 주자.
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pistolEffect, hitInfo.ImpactPoint, FRotator::ZeroRotator, true);
	}

	// 총 쏘는 애니메이션 실행
	//PlayAnimMontage(pistolMontage, 2.0f, FName(TEXT("Fire")));
	
	
	
	}

}

void APaintingGenieCharacter::SetBulletColor()
{
	//Script/Engine.Material'/Game/BluePrint/re/circle/M_REDCC.M_REDCC
	//생성자에서만 가능하기 때문에 총알을 배열로 넣어서 하자.
	ConstructorHelpers::FObjectFinder<UMaterial>tempRed(TEXT("/Script/Engine.Material'/Game/BluePrint/re/paintBullet/M_REDCC.M_REDCC'"));

	if (tempRed.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Set_Color"));
		pistolpaintArray.Add(tempRed.Object);

	}

	ConstructorHelpers::FObjectFinder<UMaterial>tempBlue(TEXT("/Script/Engine.Material'/Game/BluePrint/re/paintBullet/M_BlueSQ.M_BlueSQ'"));

	if (tempBlue.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Set_Color"));
		pistolpaintArray.Add(tempBlue.Object);
	}

	ConstructorHelpers::FObjectFinder<UMaterial>tempGreen(TEXT("/Script/Engine.Material'/Game/BluePrint/re/paintBullet/GeenSquare_Mat.GeenSquare_Mat'"));

	if (tempGreen.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Set_Color"));
		pistolpaintArray.Add(tempGreen.Object);
	}


	
	
}

void APaintingGenieCharacter::afterBulletColor()
{	
	//pbn값이 0이면 카운트의 나머지 값으로 받는다.
	pbn = (pbn + 1) % (int32)EPaintColor::COUNT; 
}

void APaintingGenieCharacter::beforeBulletColor()
{
	//pbn -1을 하면 카운트의 나머지를 값으로 받는다.
	pbn = (pbn -1 + (int32)EPaintColor::COUNT) % (int32)EPaintColor::COUNT;
}

void APaintingGenieCharacter::bulletScaleUp()
{
	BSC = BSC + 1;
	UE_LOG(LogTemp, Warning, TEXT("BSC UP :: %s"), *BSC.ToString());
}

void APaintingGenieCharacter::bulletScaleDown()
{
	BSC = BSC - 1;
	UE_LOG(LogTemp, Warning, TEXT("BSC DOWN :: %s"), *BSC.ToString());
}
