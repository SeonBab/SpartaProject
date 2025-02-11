// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WaveRow.generated.h"

USTRUCT(BlueprintType)
struct FWaveRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* ItemSpawnDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalActorSpawn;
};
