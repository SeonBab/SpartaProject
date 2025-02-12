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
	// ��ֹ� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ObstacleName;
	// � ��ֹ� Ŭ������ ��������
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> ObstacleClass;
	// ��ֹ��� ���� Ȯ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnChance;
};
