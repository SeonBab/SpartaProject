// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObstacle.h"
#include "ExplosionObstacle.generated.h"

class USphereComponent;

UCLASS()
class SPARTAPROJECT_API AExplosionObstacle : public ABaseObstacle
{
	GENERATED_BODY()
	
public:
	AExplosionObstacle();

protected:
    virtual void BeginPlay() override;

    virtual void Tick(float DeltaTime) override;

private:
    void Explode();

protected:
    // 발동 범위의 콜리전 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Component")
    USphereComponent* ExplosionCollision;
    // 폭발 범위가 보이는 데칼 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Component")
    UDecalComponent* ExplosionDecal;

    // 폭발까지 걸리는 시간 (5초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionDelay;
    // 폭발 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionRadius;
    // 폭발 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionDamage;
    // 폭발 범위가 그려지는 데칼
    float ExplosionDecalSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Effects")
    UParticleSystem* ExplosionParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Effects")
    USoundBase* ExplosionSound;
};
