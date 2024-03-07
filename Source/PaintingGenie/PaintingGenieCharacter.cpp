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
//#include "NetPlayerAnimInstance.h"


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

	// 1. �ٴڿ� ����ִ� Pistol �� ã��.
	//���͸� �迪�� �ϰ� ������ ����.
	TArray<AActor*> allActor;
	//���� �÷��� ����ƽ�� ��Ŭ���
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), allActor);
	for (int32 i = 0; i < allActor.Num(); i++)
	{
		if (allActor[i]->GetActorLabel().Contains(TEXT("Pistol")))
		{
			allPistol.Add(allActor[i]);
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

		// ������� ������ TakePistolAction�� ���ε�����.
		//�����
		EnhancedInputComponent->BindAction(TakePistolAction, ETriggerEvent::Started, this, &APaintingGenieCharacter::TakePistol);

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
	// ���࿡ ���� ��� �ִٸ�
	if (closestPistol)
	{
		//���� �տ��� ����
		DetachPistol();
		//�ǽ��� ���¸� null��
		closestPistol = nullptr;
		//����
		return;
	}

	//�Ѱ��� �Ÿ��� �ִ밪���� �ִ´�.
	// ���� ��� ���� �ʴٸ�
	float closestDist = std::numeric_limits<float>::max();
	closestPistol = nullptr;

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
		}
	}

	AttachPistol();


}

void APaintingGenieCharacter::AttachPistol()
{
	// ����� ���� ������ �Լ��� ������
	if (closestPistol == nullptr) return;

	// �������� ���� Off ��������
	auto compMesh = closestPistol->GetComponentByClass<UStaticMeshComponent>();
	compMesh->SetSimulatePhysics(false);

	// ���� ����� ����  Mesh -> GunPosition ���Ͽ� ������.
	closestPistol->AttachToComponent(compGun, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// animInstance �� �ִ� hasPistol �� true
	/*auto anim = Cast<UNetPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->hasPistol = true;*/


}

void APaintingGenieCharacter::DetachPistol()
{
	// �������� ���� On ��������
	auto compMesh = closestPistol->GetComponentByClass<UStaticMeshComponent>();
	compMesh->SetSimulatePhysics(true);
	// closestPistol �� compGun ������ ������
	closestPistol->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	// animInstance �� �ִ� hasPistol �� false
	/*auto anim = Cast<UNetPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	anim->hasPistol = false;*/
}
