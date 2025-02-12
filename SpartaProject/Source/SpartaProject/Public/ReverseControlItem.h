// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "ReverseControlItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AReverseControlItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	AReverseControlItem();

	virtual void Tick(float DeltaTime) override;

	virtual void ActivateItem(AActor* Activator) override;

private:
	void CrearReverseControll();

private:
	// ���� ���������� �ð�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	float CrearReverseControllDelay;

	// �������ͽ��� �̹���
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UTexture2D* ReverseControllTexture;

	// �̹����� ��ٿ�
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UMaterialInterface* CoolDownMaterialInstance;

	UMaterialInstanceDynamic* DynamicMaterial;

	float ActivateTime;

	UUserWidget* CurDebuffUserWidget;

	// ���ο� Ǯ���� Ÿ�̸�
	FTimerHandle ClearReverseControllTimerHandle;

	// ���ο찡 �ɸ� ���
	AActor* ActivateActor;

};
