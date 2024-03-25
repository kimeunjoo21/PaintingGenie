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

//���� �÷��� ����ƽ ��Ŭ���
#include "Kismet/GameplayStatics.h"
//�ִ� �ν��Ͻ� ��Ŭ���
#include "P4AnimInstance.h"
//�� ������Ʈ
#include "Runtime/Engine/Classes/Components/SceneComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/DecalComponent.h>
#include "Runtime/Engine/Classes/Materials/Material.h"
//�����ӿ�ũ ��Ŭ���, 
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/Actor.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h>



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

	// ���� ���� ������Ʈ ������
	compGun = CreateDefaultSubobject<USceneComponent>(TEXT("GUN"));
	compGun->SetupAttachment(GetMesh(), FName(TEXT("GunPosition")));
	compGun->SetRelativeLocation(FVector(-7.144f, 3.68f, 4.136f));
	compGun->SetRelativeRotation(FRotator(3.4f, 75.699f, 6.6424f));

	//�Ѿ� ������ ������
	SetBulletColor();

	//������ �����͸� ��������.
	SetGazePointer();

	



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

	// AnimInstance ��������
	anim = Cast<UP4AnimInstance>(GetMesh()->GetAnimInstance());

	// 1. �ٴڿ� ����ִ� Pistol �� ã��.
	//���͸� �迪�� �ϰ� ������ ����.
	TArray<AActor*> allActor;
	//���� �÷��� ����ƽ�� ��Ŭ���
	//���忡 ��ġ�� ��� Ŭ���߿��� �ǽ����̶�� �̸��� ã�Ƽ� �迭�� ����.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActor);
	for (int32 i = 0; i < allActor.Num(); i++)
	{
		//������ ���� �ǽ����̶��
		if (allActor[i]->GetActorLabel().Contains(TEXT("Pistol")))
		{
			allPistol.Add(allActor[i]);
			//UE_LOG(LogTemp, Warning, TEXT("allpistrol"));

		}
	}

	
}

void APaintingGenieCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//������ ������
	GazePointer();
	//����Ʈ �α�
	PrintNetLog();
	

	

}

void APaintingGenieCharacter::PrintNetLog()
{
	// Connection ����
	FString conStr = GetNetConnection() != nullptr ? TEXT("Valid Connect") : TEXT("InValid Connect");
	// ���� ���� Actor
	FString ownerStr = GetOwner() != nullptr ? GetOwner()->GetName() : TEXT("No Owner");
	// Role
	// ROLE_Authority : ��� ������ �� ���� �ִ� (���� ����)
	// ROLE_AutonomousProxy : ���� (Input) �� �����ϴ�.
	// ROLE_SimulatedProxy : ���⸸ (�ùķ��̼Ǹ�) �����Ѵ�.
	FString localRoleStr = UEnum::GetValueAsString<ENetRole>(GetLocalRole());
	FString remoteRoleStr = UEnum::GetValueAsString<ENetRole>(GetRemoteRole());

	FString log = FString::Printf(TEXT("Connection : %s\nOwner Name : %s\nLocalRole : %s\nRemoteRole : %s"),
		*conStr, *ownerStr, *localRoleStr, *remoteRoleStr);

	DrawDebugString(
		GetWorld(),
		GetActorLocation() + FVector::UpVector * 100,
		log,
		nullptr,
		FColor::Yellow,
		0,
		true,
		1.0);
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

		// ������� ������ TakePistolAction�� ���ε�����.
		//�����
		EnhancedInputComponent->BindAction(TakePistolAction, ETriggerEvent::Started, this, &APaintingGenieCharacter::TakePistol);

		// ������� ������ FireAction�� ���ε�����.
		// �� ��� �ܹ�
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &APaintingGenieCharacter::Fire);
		//�ѽ�� ����
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::Fire);
		//�Ҹ����� ���� �ٲٱ�
		EnhancedInputComponent->BindAction(afterBullet, ETriggerEvent::Started, this, &APaintingGenieCharacter::afterBulletColor);
		//UE_LOG(LogTemp, Warning, TEXT("afterBulletColor"));
		//�Ҹ����� ���� �ٲٱ�
		EnhancedInputComponent->BindAction(beforeBullet, ETriggerEvent::Started, this, &APaintingGenieCharacter::beforeBulletColor);
		//UE_LOG(LogTemp, Warning, TEXT("beforeBulletColor"));
		
		//�ҷ��� ũ�� �ٲٱ�
		EnhancedInputComponent->BindAction(bulletScaleUpValue, ETriggerEvent::Started, this, &APaintingGenieCharacter::bulletScaleUp);
		EnhancedInputComponent->BindAction(bulletScaleUpValue, ETriggerEvent::Triggered, this, &APaintingGenieCharacter::bulletScaleUp);
		UE_LOG(LogTemp, Warning, TEXT("bulletScaleUpValue"));
		
		//�ҷ��� ũ�� �ٲٱ�
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
	ServerRPC_TakePistol();
}

void APaintingGenieCharacter::ServerRPC_TakePistol_Implementation()
{	
	//���� �α׸� ȣ���մϴ�.
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("TakePistol server"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TakePistol client"));
	}


	// TakePistol�� ����� �����ɴϴ�.
	// ���࿡ ���� ��� �ִٸ�
	UE_LOG(LogTemp,Warning, TEXT("pistol %s"), closestPistol);
	if (closestPistol)
	{
		//���� �տ��� ����
		MultiRPC_DetachPistol();
		//�ǽ��� ���¸� null��
		//closestPistol->SetOwner(nullptr);
		closestPistol = nullptr;
		return;
	}


	// ���� ��� ���� �ʴٸ�
	//�Ѱ��� �Ÿ��� �ִ밪���� �ִ´�.
	float closestDist = std::numeric_limits<float>::max();
	closestPistol = nullptr;

	//�迭 i�� ���ǽ���
	for (int32 i = 0; i < allPistol.Num(); i++)
	{
		// 1. ��� Pistold ���� ������ �Ÿ��� ������.
		//���� ���� ��ġ�� N��° ��� �ǽ����� ��ġ�� ������ ��������.
		float dist = FVector::Distance(GetActorLocation(), allPistol[i]->GetActorLocation());

		// ���� ���� �� �ִ� ������ �ִ�?
		if (dist > takeGunDist) continue;

		// closestDist ���� dist �۴�?
		if (closestDist > dist)
		{
			// closestDist �� dist �� ����
			closestDist = dist;
			// closestPistol �� allPistol[i] �� ����
			closestPistol = allPistol[i];
			// ���� owner ����
			closestPistol->SetOwner(this);

		}
	}

	MultiRPC_AttachPistol(closestPistol);
	//AttachPistol();
}

void APaintingGenieCharacter::AttachPistol(AActor* pistol)
{
	// ����� ���� ������ �Լ��� ������
	closestPistol = pistol;
	if (closestPistol == nullptr) return;

	UE_LOG(LogTemp, Warning, TEXT("closestpistol : %s"), closestPistol);
	//UE_LOG(LogTemp, Warning, TEXT("attach pistol"));
	
	// �������� ���� Off ��������
	auto compMesh = closestPistol->GetComponentByClass<UStaticMeshComponent>();
	compMesh->SetSimulatePhysics(false);


	// ���� ����� ����  Mesh -> GunPosition ���Ͽ� ������.
	closestPistol->AttachToComponent(compGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// animInstance �� �ִ� hasPistol �� true
	anim = Cast<UP4AnimInstance>(GetMesh()->GetAnimInstance());
	anim->hasPistol = true;

	//���� ���̸� ȭ���� Ȯ�� ������.
	// bOrientRotaionToMovement ������
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// RotationYaw ������
	bUseControllerRotationYaw = true;
	// SprintArm ��ġ �ٲ�����
	CameraBoom->TargetArmLength = 100;
	CameraBoom->SetRelativeLocation(FVector(-4.33f, 33.8f, 70));
}


void APaintingGenieCharacter::MultiRPC_AttachPistol_Implementation(AActor* pistol)
{
	AttachPistol(pistol);
}

void APaintingGenieCharacter::DetachPistol()
{	

	// �������� ���� On ��������
	auto compMesh = closestPistol->GetComponentByClass<UStaticMeshComponent>();
	compMesh->SetSimulatePhysics(true);
	// closestPistol �� compGun ������ ������
	closestPistol->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	// animInstance �� �ִ� hasPistol �� false
	anim = Cast<UP4AnimInstance>(GetMesh()->GetAnimInstance());
	anim->hasPistol = false;

	//ī�޶� ����
	// bOrientRotaionToMovement ������
	GetCharacterMovement()->bOrientRotationToMovement = true;
	// RotationYaw ������
	bUseControllerRotationYaw = false;
	// SprintArm ��ġ �ٲ�����
	CameraBoom->TargetArmLength = 400;
	CameraBoom->SetRelativeLocation(FVector::ZeroVector);

	//UI�� ���̰�����.

}

void APaintingGenieCharacter::MultiRPC_DetachPistol_Implementation()
{
	
	DetachPistol();
		closestPistol = nullptr;
}

void APaintingGenieCharacter::Fire()
{
	ServerRPC_Fire();
		
}

void APaintingGenieCharacter::ServerRPC_Fire_Implementation()
{
	//Fire()�� ��������.

	// ���� ��� ���� ������ �Լ��� ������
	// �Ѿ��� 0���� �Լ��� ������
	// ������ �߿��� �Լ��� ������
	//if (closestPistol == nullptr || currBulletCnt <= 0 || isReloading) return;

	//�ǽ����� ������ ����
	if (closestPistol == nullptr)return;
	

	FHitResult hitInfo;
	FVector startPos = FollowCamera->GetComponentLocation();
	FVector endPos = startPos + FollowCamera->GetForwardVector() * 100000;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);


	MultiRPC_Fire(isHit, hitInfo.ImpactPoint, hitInfo.ImpactNormal.Rotation());
}

void APaintingGenieCharacter::MultiRPC_Fire_Implementation(bool isHit, FVector impactPoint, FRotator decalRot)
{	
	if (isHit)
	{
		//hitInfo.ImpactNormal �浹��ġ�� �����̼ǰ��� FVector�� ��ȯ�մϴ�.
		//FVector in = hitInfo.ImpactNormal;

		//�浹����� �븻���� �����̼����� ����ȯ
		//hitInfo.ImpactNormal.Rotation();
		FRotator rot = impactPoint.Rotation();
		rot = decalRot;

		//���� ��Į�� ����ġ�� �Ű�����
		/*UDecalComponent* UGameplayStatics::SpawnDecalAttached(class UMaterialInterface* DecalMaterial, FVector DecalSize, class USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, float LifeSpan)*/

		//UGameplayStatics::SpawnDecalAttached(pistolPaint,FVector(10) params,);
		//�������� (��ġ, Ÿ��, ũ��, ��ġ, ����, �ð�(0=������))
		//->SetSortOrder(order);
		//->SetSortOrder(order); �� ��Ʈ������ ���ؼ� ���̾ �ֻ����� �ø��ϴ�.
		// order++; ������ �����մϴ�.  

		//pbn = scale
		//bsc = decalsize FVector(50)
		//rot = hitInfo.ImpactNormal.Rotation();
		UMaterialInstanceDynamic* mat = UMaterialInstanceDynamic::Create(pistolpaintArray[pbn], this);
		mat->SetVectorParameterValue(FName(TEXT("Param")), color);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *color.ToString());
		UGameplayStatics::SpawnDecalAtLocation(GetWorld(), mat, FVector(BSC), impactPoint, rot, 0)->SetSortOrder(order);
		order++;

		//UE_LOG(LogTemp, Warning, TEXT("Spawn Decal"));

		//�浹�� ���� ȿ�� ����.
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), pistolEffect, hitInfo.ImpactPoint, FRotator::ZeroRotator, true);
	}

	// �� ��� �ִϸ��̼� ����
	//PlayAnimMontage(pistolMontage, 2.0f, FName(TEXT("Fire")));
}



void APaintingGenieCharacter::SetBulletColor()
{
	//Script/Engine.Material'/Game/BluePrint/re/circle/M_REDCC.M_REDCC
	//�����ڿ����� �����ϱ� ������ �Ѿ��� �迭�� �־ ����.
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

	ConstructorHelpers::FObjectFinder<UMaterial>tempGreen(TEXT("/Script/Engine.Material'/Game/KEJ/M_Space.M_Space'"));

	if (tempGreen.Succeeded())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Set_Color"));
		pistolpaintArray.Add(tempGreen.Object);
	}


	//MultiRPC_SetBulletColor();
	
}

void APaintingGenieCharacter::MultiRPC_SetBulletColor_Implementation()
{
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
	//pbn���� 0�̸� ī��Ʈ�� ������ ������ �޴´�.
	//pbn = (pbn + 1) % (int32)EPaintColor::COUNT;
	
	ServerRPC_afterBulletColor();
	
	
}

void APaintingGenieCharacter::ServerRPC_afterBulletColor_Implementation()
{
	//pbn���� 0�̸� ī��Ʈ�� ������ ������ �޴´�.
	//pbn = (pbn + 1) % (int32)EPaintColor::COUNT;
	
		//pbn = pbn + 1;
		////UE_LOG(LogTemp, Warning, TEXT(" server abc : %d"), pbn);

		//if (pbn > pistolpaintArray.Num() - 1)
		//{
		//	pbn = 0;
		//	
		//}
		MultiRPC_afterBulletColor();
}

void APaintingGenieCharacter::MultiRPC_afterBulletColor_Implementation()
{
	pbn = pbn + 1;
	
	UE_LOG(LogTemp, Warning, TEXT(" multi abc : %d"), pbn);

	if (pbn > pistolpaintArray.Num() - 1)
	{
		pbn = 0;

	}
}

void APaintingGenieCharacter::beforeBulletColor()
{
	//pbn -1�� �ϸ� ī��Ʈ�� �������� ������ �޴´�.
	//pbn = (pbn -1 + (int32)EPaintColor::COUNT) % (int32)EPaintColor::COUNT;
	/*pbn = pbn - 1;

	if (pbn < 0)
	{
		pbn = pistolpaintArray.Num()-1;
	}*/
	ServerRPC_beforeBulletColor();
	
}

void APaintingGenieCharacter::ServerRPC_beforeBulletColor_Implementation()
{
	/*pbn = pbn - 1;

	if (pbn < 0)
	{
		pbn = pistolpaintArray.Num() - 1;
	}*/

	MultiRPC_beforeBulletColor();
}

void APaintingGenieCharacter::MultiRPC_beforeBulletColor_Implementation()
{
	pbn = pbn - 1;

	if (pbn < 0)
	{
		pbn = pistolpaintArray.Num() - 1;
	}
}

void APaintingGenieCharacter::bulletScaleUp()
{
	ServerRPC_bulletScaleUp();
}

void APaintingGenieCharacter::ServerRPC_bulletScaleUp_Implementation()
{
	MultiRPC_bulletScaleUp();
}

void APaintingGenieCharacter::MultiRPC_bulletScaleUp_Implementation()
{
	//BulletScaleChange
	BSC = BSC + 1;
	UE_LOG(LogTemp, Warning, TEXT("BSC UP :: %s"), *BSC.ToString());
}

void APaintingGenieCharacter::bulletScaleDown()
{
	ServerRPC_bulletScaleDown();
}

void APaintingGenieCharacter::ServerRPC_bulletScaleDown_Implementation()
{
	MultiRPC_bulletScaleDown();
}

void APaintingGenieCharacter::MultiRPC_bulletScaleDown_Implementation()
{
	//���� 0���� �۰ų� ������ ����
	if (BSC.Length() <= 0) return;
	BSC = BSC - 1;


	UE_LOG(LogTemp, Warning, TEXT("BSC DOWN :: %s"), *BSC.ToString());
}

void APaintingGenieCharacter::SetGazePointer()
{
	gazePointer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gaze Pointer Mesh"));
	gazePointer->SetupAttachment(RootComponent);
	gazePointer->SetWorldScale3D(FVector(0.1f));
	gazePointer->SetWorldLocation(FVector(300, 0, 0));
	gazePointer->SetWorldRotation(FRotator(0, 90, 90));
	gazePointer->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ConstructorHelpers::FObjectFinder<UStaticMesh>tempGaze(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Plane.Plane'"));
	if (tempGaze.Succeeded())
	{
		gazePointer->SetStaticMesh(tempGaze.Object);
		//�ε�����ȣ, ��Ƽ����
		//gazePointer->SetMaterial(0, mateGaze.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial>mateGaze(TEXT("/ Script / Engine.Material'/Game/BluePrint/re/gazepointer/M_GazePointer.M_GazePointer'"));
	if (mateGaze.Succeeded())
	{	
		//�ε�����ȣ, ��Ƽ����
		gazePointer->SetMaterial(0, mateGaze.Object);
	}



}

void APaintingGenieCharacter::GazePointer()
{
	//���� ��Ҵ�?
	if (closestPistol)
	{
		FHitResult hitInfo;
		FVector startPos = FollowCamera->GetComponentLocation();
		FVector endPos = startPos + FollowCamera->GetForwardVector() * maxSight;
		//FVector endPos = startPos + FollowCamera->GetForwardVector() * 100000;
		//FVector camForward = FRotationMatrix(FollowCamera->GetComponentRotation()).GetUnitAxis(EAxis::X);
		//FVector endPos = startPos + camForward * 100000;
		FCollisionQueryParams params;
		params.AddIgnoredActor(this);
		bool isHit = GetWorld()->LineTraceSingleByChannel(hitInfo, startPos, endPos, ECollisionChannel::ECC_Visibility, params);

		if (isHit)
		{
			gazePointer->SetVisibility(true);
			gazePointer->SetWorldLocation(hitInfo.Location);
			gazePointer->SetWorldScale3D(BSC * 0.05f);

		}
		else
		{
			gazePointer->SetVisibility(false);
		}
		//FVector calcSize = FMath::Lerp(FVector(minSize), FVector(maxSize), rate);
		//���� ��Ҵٸ� ���
		//UE_LOG(LogTemp, Warning,TEXT("Grab pistol"))	
	}
	
}
