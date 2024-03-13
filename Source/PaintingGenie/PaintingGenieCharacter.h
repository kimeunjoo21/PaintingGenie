// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PaintingGenieCharacter.generated.h"


//페인트 컬러를 이넘 클랙스로 정의하고 새로운 머티리얼이 추가 될때마다 다음차순으로 넣어준다.
//기본 RGB 컬러 적용됨.
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

	//총잡기 인풋을 추가하자
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TakePistolAction;
	//총 쏘기 인풋
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* FireAction;

	//다음 총알색 바꾸기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* afterBullet;
	//이전 총알색 바꾸기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* beforeBullet;
	//머티리얼의 크기를 바꾸자.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* bulletScaleUpValue;
	//머티리얼의 크기를 바꾸자.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* bulletScaleDownValue;





public:
	APaintingGenieCharacter();
	

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	//총을 잡자
	void TakePistol();
	//총을 소켓에 붙이자
	void AttachPistol();
	//총을 때자
	void DetachPistol();

public:
	//총을쏘자
	void Fire();
	//	색깔을 설정하자
	void SetBulletColor();
	//다음 색깔로 바꾸자
	void afterBulletColor();
	//색 배열변수
	UPROPERTY(BlueprintReadWrite)
	int32 pbn = 0;
	//TArray<class UMaterialInterface> pbn;
	//오더 차상위를 지정합니다.
	int32 order = 0;
	//이전 색깔로 바꾸자
	void beforeBulletColor();
	//불렛 스케일을 키울수 있을까?
	UPROPERTY(EditAnywhere)
	//불렛 크기 초기 값 20
	FVector BSC = FVector(20);
	void bulletScaleUp();
	void bulletScaleDown();
	
	//불릿 크기를 미리보기 할수 있을까?
	//최대시야거리
	UPROPERTY(EditAnywhere, Category="MySettings")
	float maxSight = 1000.0f;
	//최소크기
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float minSize = 1.0f;
	//최대크기 BSC와 동기화
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float maxSize= BSC.Length();
	//줄어드는값
	float rate = 1.0f;
	
	//매쉬가 필요함.
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
	//애님 인스턴트를 가져오자.
	UPROPERTY()
	class UP4AnimInstance* anim;
	
	//총을 붙일 컴포넌트 추가하자.
	UPROPERTY(EditAnywhere, Category = "MySettings|Components")
	class USceneComponent* compGun;
	
	//액터클래스를 배열로 선언하자.
	UPROPERTY(VisibleInstanceOnly)
	TArray<class AActor*> allPistol;

	//총과의 거리
	UPROPERTY(EditAnywhere)
	float takeGunDist = 200;

	UPROPERTY()
	AActor* closestPistol;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* pistolEffect;

	//피스톨 페인트 테스트
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* pistolPaint;

	//페인트를 배열로 지정하자.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> pistolpaintArray;

	
	
	

};

