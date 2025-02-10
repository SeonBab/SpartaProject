// Fill out your copyright notice in the Description page of Project Settings.


#include "HealingItem.h"
#include "SpartaCharacter.h"

AHealingItem::AHealingItem()
{
	HealAmount = 20;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	// �÷��̾� ĳ������ ü���� 20��ŭ ȸ����Ű�� ���� ���� ����
	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			// ĳ������ ü���� ȸ��
			PlayerCharacter->AddHealth(HealAmount);
		}

		DestroyItem();
	}
}
