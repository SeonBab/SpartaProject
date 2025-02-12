// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ObstacleSpawnRow.generated.h"


USTRUCT(BlueprintType)
struct FObstacleSpawnRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	// 장애물 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObstacleName;
	// 어떤 장애물 클래스를 스폰할지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObstacleClass;
	// 장애물의 스폰 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
