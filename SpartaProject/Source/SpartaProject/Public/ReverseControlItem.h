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
	// 반전 해제까지의 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	float CrearReverseControllDelay;

	// 스테이터스의 이미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UTexture2D* ReverseControllTexture;

	// 이미지의 쿨다운
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UMaterialInterface* CoolDownMaterialInstance;

	UMaterialInstanceDynamic* DynamicMaterial;

	float ActivateTime;

	UUserWidget* CurDebuffUserWidget;

	// 슬로우 풀리는 타이머
	FTimerHandle ClearReverseControllTimerHandle;

	// 슬로우가 걸린 대상
	AActor* ActivateActor;

};
