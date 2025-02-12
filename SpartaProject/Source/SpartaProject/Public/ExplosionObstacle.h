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
    // �ߵ� ������ �ݸ��� ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Component")
    USphereComponent* ExplosionCollision;
    // ���� ������ ���̴� ��Į ������Ʈ
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Obstacle|Component")
    UDecalComponent* ExplosionDecal;

    // ���߱��� �ɸ��� �ð� (5��)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionDelay;
    // ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionRadius;
    // ���� ������
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosion")
    float ExplosionDamage;
    // ���� ������ �׷����� ��Į
    float ExplosionDecalSize;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Effects")
    UParticleSystem* ExplosionParticle;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacle|Effects")
    USoundBase* ExplosionSound;
};
