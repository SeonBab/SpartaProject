// Fill out your copyright notice in the Description page of Project Settings.


#include "BigCoinItem.h"

ABigCoinItem::ABigCoinItem()
{
	PointValue = 50;
	ItemType = "BigCoin";

	Scene->SetRelativeScale3D(FVector(1.5f, 1.5f, 1.5f));
}

void ABigCoinItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	// �� ���θ��� �߰� ����(����Ʈ, ���� ��� ��)�� ���⼭ �߰��� �� ����

}
