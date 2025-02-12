// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SlowingItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ASlowingItem();

	virtual void Tick(float DeltaTime) override;

	virtual void ActivateItem(AActor* Activator) override;

private:
	void ClearSlow();

private:
	// ���ο� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slow", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", AllowPrivateAccess = true))
	float SlowSpeed;

	// ������ ���������� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slow", meta = (AllowPrivateAccess = true))
	float ClearSlowDelay;

	// �������ͽ��� �̹���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UTexture2D* SlowTexture;

	// �̹����� ��ٿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UMaterialInterface* CoolDownMaterialInstance;

	UMaterialInstanceDynamic* DynamicMaterial;

	float ActivateTime;

	UUserWidget* CurDebuffUserWidget;

	// ���ο� Ǯ���� Ÿ�̸�
	FTimerHandle ClearSlowTimerHandle;

	// ���ο찡 �ɸ� ���
	AActor* ActivateActor;
};
