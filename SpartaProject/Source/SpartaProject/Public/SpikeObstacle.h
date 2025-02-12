// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseObstacle.h"
#include "SpikeObstacle.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ASpikeObstacle : public ABaseObstacle
{
	GENERATED_BODY()

public:
	ASpikeObstacle();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spike")
	float SpikeDamage;

	// 변경되는 위치 값 ( -1 ~ 1 * 100.f)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spike")
	float LocationRange = 100.f;
	// 위치 변경 속도 (2초 마다)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spike")
	float LocationSpeed = 0.5f;
};
