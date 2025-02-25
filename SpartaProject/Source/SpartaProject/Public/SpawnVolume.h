// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "ObstacleSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

	virtual void BeginPlay() override;

	// 스폰 볼륨 내부에서 무작위 좌표를 얻어오는 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;
	// 스폰 볼륨 내부에서 무작위 X,Y 좌표를 얻어오는 함수(Z축은 Volume의 최소값)
	FVector GetRandomXYPointInVolume() const;

	// 특정 아이템 클래스를 스폰하는 함수
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);
	// 특정 액터 클래스를 스폰하는 함수
	AActor* SpawnObstacle(TSubclassOf<AActor> ItemClass);

	// 스폰 해야하는 아이템 클래스를 선택하는 함수
	FItemSpawnRow* GetRandomItem(const UDataTable* SelectedRow) const;
	// 스폰 해야하는 장애물 클래스를 선택하는 함수
	FObstacleSpawnRow* GetRandomObstacle(const UDataTable* SelectedRow) const;

	// 랜덤으로 아이템 클래스를 스폰을 하는 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem(const UDataTable* ItemSpawnDataTable);
	// 랜덤으로 장애물 클래스를 스폰하는 함수
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomObstacle(const UDataTable* ObstacleSpawnDataTable);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	// 스폰 영역을 담당할 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;
};
