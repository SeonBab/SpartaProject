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
	// 슬로우 배율
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slow", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", AllowPrivateAccess = true))
	float SlowSpeed;

	// 슬로후 해제까지의 시간
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slow", meta = (AllowPrivateAccess = true))
	float ClearSlowDelay;

	// 스테이터스의 이미지
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UTexture2D* SlowTexture;

	// 이미지의 쿨다운
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ReverseControll", meta = (AllowPrivateAccess = true))
	UMaterialInterface* CoolDownMaterialInstance;

	UMaterialInstanceDynamic* DynamicMaterial;

	float ActivateTime;

	UUserWidget* CurDebuffUserWidget;

	// 슬로우 풀리는 타이머
	FTimerHandle ClearSlowTimerHandle;

	// 슬로우가 걸린 대상
	AActor* ActivateActor;
};
