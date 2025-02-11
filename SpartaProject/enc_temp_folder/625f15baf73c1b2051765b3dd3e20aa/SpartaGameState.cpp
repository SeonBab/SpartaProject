// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "WaveRow.h"

ASpartaGameState::ASpartaGameState()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 10.0f; // �� ������ 30��
	CurrentWaveIndex = 0;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	// ���� ���� �� ù �������� ����
	WaveCheck();

	// HUD�� ���� Ÿ�̸� ����
	GetWorldTimerManager().SetTimer(HUDTimerHandle, this, &ASpartaGameState::UpdateHUD, 0.1f, true);
}

void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::OnGameOver()
{
	// ���̺갡 ���� ���
	// ���̺� �� �ʱ�ȭ
	CurrentWaveIndex = 0;
	// ���� ���� �ʱ�ȭ
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}

void ASpartaGameState::WaveCheck()
{
	UDataTable* WaveDataTable = nullptr;
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			WaveDataTable = SpartaGameInstance->GetWaveDataTable();
		}
	}

	if (!WaveDataTable) return;

	// TODO
	// ������ ���̺��� ���̺꿡 ���� ������ �����ɴϴ�.  
	// ���� ���̺꿡 ���� ������ �ִ� ��� ���̺� ������ ȣ���մϴ�.
	// ���� ���̺꿡 ���� ������ ���� ��� ���� ������ ȣ���մϴ�.

	++CurrentWaveIndex;

	//���� ���̺��� Row(��) ��������
	FWaveRow* CurRows;
	static const FString ContextString(TEXT("WaveContext"));
	CurRows = WaveDataTable->FindRow<FWaveRow>(FName(*FString::FromInt(CurrentWaveIndex)), ContextString);
	
	if (CurRows)
	{
		//���� ���̺꿡 �����ؾ��ϴ� �������� ������ �����;���
		int32 CurTotalActorSpawn = CurRows->TotalActorSpawn;
		UDataTable* CurItemSpawnDataTable = CurRows->ItemSpawnDataTable;
		StartWave(CurTotalActorSpawn, CurItemSpawnDataTable);

		// Ư�� �ð� �Ŀ� WaveCheck()�� ȣ��ǵ��� Ÿ�̸� ����
		GetWorldTimerManager().SetTimer(WavelTimerHandle, this, &ASpartaGameState::WaveCheck, LevelDuration, false);
	}
	else
	{
		// Ÿ�̸� ����
		GetWorldTimerManager().ClearTimer(WavelTimerHandle);

		OnGameOver();
	}
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	// ���� �������� ������ ������ ���� �ֿ��ٸ� ��� ���� ����
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		WaveCheck();
	}
}

void ASpartaGameState::StartWave(const int32 TotalActorSpawn, const UDataTable* ItemSpawnDataTable)
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	// ���� �ʿ� ��ġ�� ��� SpawnVolume�� ã�� �������� ����
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	for (int32 i = 0; i < TotalActorSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem(ItemSpawnDataTable);
				// ���� ������ ���Ͱ� ���� Ÿ���̶�� SpawnedCoinCount ����
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}
}

void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WavelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex)));
				}
			}
		}
	}
}
