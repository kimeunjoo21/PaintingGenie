// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PaintingGenieCharacter.generated.h"


//����Ʈ �÷��� �̳� Ŭ������ �����ϰ� ���ο� ��Ƽ������ �߰� �ɶ����� ������������ �־��ش�.
//�⺻ RGB �÷� �����.
UENUM(BlueprintType)
enum class EPaintColor : uint8 
{	
	RED,
	BLUE,
	GREEN,
	//YELLOW,
	COUNT
};

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APaintingGenieCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	//����� ��ǲ�� �߰�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TakePistolAction;
	//�� ��� ��ǲ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	//���� �Ѿ˻� �ٲٱ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* afterBullet;
	//���� �Ѿ˻� �ٲٱ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* beforeBullet;
	//��Ƽ������ ũ�⸦ �ٲ���.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* bulletScaleUpValue;
	//��Ƽ������ ũ�⸦ �ٲ���.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* bulletScaleDownValue;





public:
	APaintingGenieCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	//���� ����
	void TakePistol();
	//���� ���Ͽ� ������
	void AttachPistol();
	//���� ����
	void DetachPistol();

public:
	//��������
	void Fire();
	//	������ ��������
	void SetBulletColor();
	//���� ����� �ٲ���
	void afterBulletColor();
	//�� �迭����
	UPROPERTY(BlueprintReadWrite)
	int32 pbn = 0;
	//TArray<class UMaterialInterface> pbn;
	//���� �������� �����մϴ�.
	int32 order = 0;
	//���� ����� �ٲ���
	void beforeBulletColor();
	//�ҷ� �������� Ű��� ������?
	UPROPERTY(EditAnywhere)
	//�ҷ� ũ�� �ʱ� �� 20
	FVector BSC = FVector(20);
	void bulletScaleUp();
	void bulletScaleDown();
	
	//�Ҹ� ũ�⸦ �̸����� �Ҽ� ������?
	//�ִ�þ߰Ÿ�
	UPROPERTY(EditAnywhere, Category="MySettings")
	float maxSight = 1000.0f;
	//�ּ�ũ��
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float minSize = 1.0f;
	//�ִ�ũ�� BSC�� ����ȭ
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float maxSize= BSC.Length();
	//�پ��°�
	float rate = 1.0f;
	
	//�Ž��� �ʿ���.
	UPROPERTY(VisibleAnywhere, Category = "MySettings|Components")
	class UStaticMeshComponent* gazePointer;

	//bool CheckShowObjects();






protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;


public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	//�ִ� �ν���Ʈ�� ��������.
	UPROPERTY()
	class UP4AnimInstance* anim;
	
	//���� ���� ������Ʈ �߰�����.
	UPROPERTY(EditAnywhere, Category = "MySettings|Components")
	class USceneComponent* compGun;
	
	//����Ŭ������ �迭�� ��������.
	UPROPERTY(VisibleInstanceOnly)
	TArray<class AActor*> allPistol;

	//�Ѱ��� �Ÿ�
	UPROPERTY(EditAnywhere)
	float takeGunDist = 200;

	UPROPERTY()
	AActor* closestPistol;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* pistolEffect;

	//�ǽ��� ����Ʈ �׽�Ʈ
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* pistolPaint;

	//����Ʈ�� �迭�� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> pistolpaintArray;

	
	
	

};

