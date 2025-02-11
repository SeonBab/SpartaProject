// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	USpartaGameInstance();

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);

	UDataTable* GetWaveDataTable();

	// ���� ��ü ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;

protected:
	// ���̺� ����
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "GameData")
	UDataTable* WaveDataTable;
};
