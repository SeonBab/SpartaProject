// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpartaCharacter.generated.h"

 // ���� ����
class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
// Enhanced Input���� �׼� ���� ���� �� ����ϴ� ����ü
struct FInputActionValue;

UCLASS()
class SPARTAPROJECT_API ASpartaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASpartaCharacter();

	// ���� ü���� �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetHealth() const;
	// ü���� ȸ����Ű�� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Health")
	void AddHealth(float Amount);
	// �ִ� ü���� �������� �Լ�
	UFUNCTION(BlueprintPure, Category = "Health")
	int32 GetMaxHealth() const;

	// ���ο� ȿ�� ����
	void AddSlowSpeed(float SlowSpeed);
	void RemoveSlowSpeed(float SlowSpeed);

	// �Է� ���� ȿ�� ����
	void SetIsReverseMoveInput(bool NewbIsReverseMoveInput);

protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IA_Move�� IA_Jump ���� ó���� �Լ� ����
	// Enhanced Input���� �׼� ���� FInputActionValue�� ���޵˴ϴ�.
	UFUNCTION()
	void Move(const FInputActionValue& Value);
	UFUNCTION()
	void StartJump(const FInputActionValue& Value);
	UFUNCTION()
	void StopJump(const FInputActionValue& Value);
	UFUNCTION()
	void Look(const FInputActionValue& Value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& Value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& Value);

	// ������ ó�� �Լ��μ� �ܺηκ��� �������� ���� �� ȣ���
	// AActor�� TakeDamage()�� �������̵�
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// ��� ó�� �Լ� (ü���� 0 ���ϰ� �Ǿ��� �� ȣ��)
	UFUNCTION(BlueprintCallable, Category = "Health")
	virtual void OnDeath();

	void UpdateOverheadHP();

protected:
	// ������ �� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	// ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;
	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	// �̵� �ӵ� ���� ������Ƽ��
	// �⺻ �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NormalSpeed;
	// �⺻ �ȱ� �ӵ� ��� �� ��� ������ �޸��� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeedMultiplier;
	// ���� �޸��� �ӵ�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float SprintSpeed;

	// �Է� ����
	bool bIsReverseMoveInput;

	// �ִ� ü��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;
	// ���� ü��
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
};
