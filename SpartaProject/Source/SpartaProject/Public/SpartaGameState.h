// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()
	
public:
	ASpartaGameState();

	virtual void BeginPlay() override;

    // ������ �߰����ִ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

	// ������ ������ ������ �� (��� ���� ����) ����Ǵ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	// ������ �ֿ��� �� ȣ��
	void OnCoinCollected();

	// ������ ������ ��, ���̺갡 �� �� ������ �� ȣ��
	void WaveCheck();

	// ������ ���� �� Ÿ�̸� ����
	void StartWave(const int32 ItemToSpawn, const UDataTable* ItemSpawnDataTable);

	void UpdateHUD();

	// ���� �������� ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	// �÷��̾ ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	// ���� ���̺�
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWaveIndex;

	// �� ������ �����Ǵ� �ð� (�� ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float LevelDuration;

private:
	// �� ������ ������ ������ �ð��� �帣���� �����ϴ� Ÿ�̸�
	FTimerHandle WavelTimerHandle;

	FTimerHandle HUDTimerHandle;
};
