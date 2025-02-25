// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "MineItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API AMineItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
    AMineItem();

    virtual void ActivateItem(AActor* Activator) override;

    void Explode();

    // 발동 범위의 액터 확인용 콜리전 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
    USphereComponent* ExplosionCollision;

    // 지뢰 발동 타이머
    FTimerHandle ExplosionTimerHandle;

    bool bHasExploded;

    // 폭발까지 걸리는 시간 (5초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDelay;
    // 폭발 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionRadius;
    // 폭발 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mine")
    float ExplosionDamage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    UParticleSystem* ExplosionParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Effects")
    USoundBase* ExplosionSound;
};
