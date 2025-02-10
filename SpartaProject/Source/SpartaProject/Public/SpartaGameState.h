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

    // ���� ������ �д� �Լ�
    UFUNCTION(BlueprintPure, Category = "Score")
    int32 GetScore() const;
    // ������ �߰����ִ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Score")
    void AddScore(int32 Amount);

	// ������ ������ ������ �� (��� ���� ����) ����Ǵ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();

	// ������ ������ ��, ������ ���� �� Ÿ�̸� ����
	void StartLevel();
	// ���� ���� �ð��� ����Ǿ��� �� ȣ��
	void OnLevelTimeUp();
	// ������ �ֿ��� �� ȣ��
	void OnCoinCollected();
	// ������ ���� �����ϰ� ���� ������ �̵�
	void EndLevel();

	void UpdateHUD();

	// ���� �������� ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	// �÷��̾ ������ ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	// ���� ������ �����ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Score")
	int32 Score;
	// ��ü ������ ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	// ���� ���� ���� ���� �ε���
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	// ���� ���� �� �̸� �迭. ���� �ִ� �ε����� ���ʴ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	// �� ������ �����Ǵ� �ð� (�� ����)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;

private:
	// �� ������ ������ ������ �ð��� �帣���� �����ϴ� Ÿ�̸�
	FTimerHandle LevelTimerHandle;

	FTimerHandle HUDTimerHandle;
};
