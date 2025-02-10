// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "CoinItem.generated.h"

/**
 * 
 */
UCLASS()
class SPARTAPROJECT_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ACoinItem();

protected:
	// �θ� Ŭ�������� ��ӹ��� ActivateItem �Լ��� �������̵�
	virtual void ActivateItem(AActor* Activator) override;

	// ���� ȹ�� �� ���� ���� (�ڽ� Ŭ�������� ��ӹ޾� ���� �ٲ��� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;
};
