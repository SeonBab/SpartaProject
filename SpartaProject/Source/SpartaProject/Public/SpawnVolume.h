// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawnRow.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;

UCLASS()
class SPARTAPROJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnVolume();

	virtual void BeginPlay() override;

	// ���� ���� ���ο��� ������ ��ǥ�� ������ �Լ�
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	FVector GetRandomPointInVolume() const;
	// Ư�� ������ Ŭ������ �����ϴ� �Լ�
	AActor* SpawnItem(TSubclassOf<AActor> ItemClass);

	// ���� �ؾ��ϴ� ������ Ŭ������ �����ϴ� �Լ�
	FItemSpawnRow* GetRandomItem() const;
	// �������� ������ Ŭ������ ������ �ϴ� �Լ�
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	AActor* SpawnRandomItem();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	USceneComponent* Scene;
	// ���� ������ ����� �ڽ� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning")
	UBoxComponent* SpawningBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	UDataTable* ItemDataTable;
};
