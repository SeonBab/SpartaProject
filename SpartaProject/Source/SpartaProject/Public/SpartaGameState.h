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

    // 점수를 추가해주는 함수
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

	// 게임이 완전히 끝났을 때 (모든 레벨 종료) 실행되는 함수
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	
	// 코인을 주웠을 때 호출
	void OnCoinCollected();

	// 레벨을 시작할 때, 웨이브가 한 번 끝났을 때 호출
	void WaveCheck();

	// 아이템 스폰 및 타이머 설정
	void StartWave(const int32 ItemToSpawn, const UDataTable* ItemSpawnDataTable);

	void UpdateHUD();

	// 현재 레벨에서 스폰된 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	// 플레이어가 수집한 코인 개수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;

	// 현재 웨이브
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWaveIndex;

	// 각 레벨이 유지되는 시간 (초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float LevelDuration;

private:
	// 매 레벨이 끝나기 전까지 시간이 흐르도록 관리하는 타이머
	FTimerHandle WavelTimerHandle;

	FTimerHandle HUDTimerHandle;
};
