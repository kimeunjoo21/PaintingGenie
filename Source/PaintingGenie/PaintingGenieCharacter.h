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
	//���͸� ��������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* spawnVoteActor;
	
	//�÷��̾��� ��ġ�� ��ŸƮ ��ġ�� �̵�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetPlayerLocationStartPosition;
	
	
	
	
	
	//���͸�������
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* removeBulletActor;



public:
	APaintingGenieCharacter();
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	//���� ����
	void TakePistol();
	//���� ����ȭ�� �غ���.
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakePistol();


	//���� ���Ͽ� ������
	void AttachPistol(AActor* pistol);
	//��Ƽ ĳ��Ʈ ����ȭ
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_AttachPistol(AActor* pistol);

	//���� ����
	void DetachPistol();
	//���� ĳ��Ʈ ����ȭ
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DetachPistol();

public:
	//��������
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire(FLinearColor bulletColor);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Fire(bool isHit, FVector impactPoint, FRotator decalRot, FLinearColor bulletColor);

	//	������ ��������
	void SetBulletColor();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetBulletColor();
	

	//���� ����� �ٲ���
	void afterBulletColor();
	UFUNCTION(Server, Reliable)
	void ServerRPC_afterBulletColor();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_afterBulletColor();

	//�� �迭����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 pbn = 0;
	//TArray<class UMaterialInterface> pbn;
	//���� �������� �����մϴ�.
	int32 order = 0;
	
	//���� ����� �ٲ���
	void beforeBulletColor();
	UFUNCTION(Server, Reliable)
	void ServerRPC_beforeBulletColor();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_beforeBulletColor();

	//�ҷ� �������� Ű��� ������?
	UPROPERTY(EditAnywhere)
	//�ҷ� ũ�� �ʱ� �� 20
	FVector BSC = FVector(20);
	void bulletScaleUp();
	UFUNCTION(Server, Reliable)
	void ServerRPC_bulletScaleUp();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_bulletScaleUp();

	void bulletScaleDown();
	UFUNCTION(Server, Reliable)
	void ServerRPC_bulletScaleDown();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_bulletScaleDown();
	
	//�Ҹ� ũ�⸦ �̸����� �Ҽ� ������?
	void SetGazePointer();
	
	//�ִ�þ߰Ÿ�
	UPROPERTY(EditAnywhere, Category="MySettings")
	float maxSight = 10000.0f;
	//�ּ�ũ��
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float minSize = 1.0f;
	//�ִ�ũ�� BSC�� ����ȭ
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float maxSize= BSC.Length();
	//�پ��°�
	float rate = 1.0f;
	//������ ������ �Ž��� �ʿ���.
	UPROPERTY(VisibleAnywhere, Category = "MySettings|Components")
	class UStaticMeshComponent* gazePointer;
	void GazePointer();

	UFUNCTION()
	void SpawnVoteActor();
	UFUNCTION(Server, Reliable)
	void ServerRPC_SpawnVoteActor(FVector pos, FRotator rot);

	UFUNCTION()
	void SetGenieLocation();


	UFUNCTION()
	void Remove();

//��ǲ ����
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaSeconds) override;

	void PrintNetLog();


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
	
	//���� �Ž��� ����������
	//UPROPERTY()
	//class UStaticMeshComponent * compMesh;

	//UPROPERTY(EditAnywhere)
	//class UParticleSystem* pistolEffect;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* pistolEffect;

	//�ǽ��� ����Ʈ �׽�Ʈ
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* pistolPaint;

	//����Ʈ�� �迭�� ��������.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> pistolpaintArray;

	//�����÷��� ������.
	UPROPERTY(BlueprintReadWrite)
	FLinearColor color = FLinearColor(1, 0, 0, 1);

	//��Ƽ���� ���̳��� ����
	class UMaterialInstanceDynamic* mat;

	//���͸� �����ͼ� ��ȯ����
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AVoteActor> spawnFactory;

	
	

};

