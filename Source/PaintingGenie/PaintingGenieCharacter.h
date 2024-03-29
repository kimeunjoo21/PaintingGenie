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
	//액터를 스폰하자
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* spawnVoteActor;
	
	//플레이어의 위치를 스타트 위치로 이동
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SetPlayerLocationStartPosition;
	
	
	
	
	
	//액터를지우자
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* removeBulletActor;



public:
	APaintingGenieCharacter();
	
protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
			
	//총을 잡자
	void TakePistol();
	//서버 동기화를 해보자.
	UFUNCTION(Server, Reliable)
	void ServerRPC_TakePistol();


	//총을 소켓에 붙이자
	void AttachPistol(AActor* pistol);
	//멀티 캐스트 동기화
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_AttachPistol(AActor* pistol);

	//총을 때자
	void DetachPistol();
	//멀태 캐스트 동기화
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_DetachPistol();

public:
	//총을쏘자
	UFUNCTION(BlueprintCallable)
	void Fire();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Fire(FLinearColor bulletColor);
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_Fire(bool isHit, FVector impactPoint, FRotator decalRot, FLinearColor bulletColor);

	//	색깔을 설정하자
	void SetBulletColor();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_SetBulletColor();
	

	//다음 색깔로 바꾸자
	void afterBulletColor();
	UFUNCTION(Server, Reliable)
	void ServerRPC_afterBulletColor();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_afterBulletColor();

	//색 배열변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 pbn = 0;
	//TArray<class UMaterialInterface> pbn;
	//오더 차상위를 지정합니다.
	int32 order = 0;
	
	//이전 색깔로 바꾸자
	void beforeBulletColor();
	UFUNCTION(Server, Reliable)
	void ServerRPC_beforeBulletColor();
	UFUNCTION(NetMulticast, Reliable)
	void MultiRPC_beforeBulletColor();

	//불렛 스케일을 키울수 있을까?
	UPROPERTY(EditAnywhere)
	//불렛 크기 초기 값 20
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
	
	//불릿 크기를 미리보기 할수 있을까?
	void SetGazePointer();
	
	//최대시야거리
	UPROPERTY(EditAnywhere, Category="MySettings")
	float maxSight = 10000.0f;
	//최소크기
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float minSize = 1.0f;
	//최대크기 BSC와 동기화
	UPROPERTY(EditAnywhere, Category = "MySettings")
	float maxSize= BSC.Length();
	//줄어드는값
	float rate = 1.0f;
	//게이지 포인터 매쉬가 필요함.
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

//인풋 세팅
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
	
	//컴프 매쉬를 전역변수로
	//UPROPERTY()
	//class UStaticMeshComponent * compMesh;

	//UPROPERTY(EditAnywhere)
	//class UParticleSystem* pistolEffect;
	
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* pistolEffect;

	//피스톨 페인트 테스트
	UPROPERTY(EditAnywhere)
	class UMaterialInterface* pistolPaint;

	//페인트를 배열로 지정하자.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class UMaterialInterface*> pistolpaintArray;

	//라인컬러를 정하자.
	UPROPERTY(BlueprintReadWrite)
	FLinearColor color = FLinearColor(1, 0, 0, 1);

	//머티리얼 다이나믹 설정
	class UMaterialInstanceDynamic* mat;

	//액터를 가져와서 소환하자
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AVoteActor> spawnFactory;

	
	

};

