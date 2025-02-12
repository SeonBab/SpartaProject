// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "SpartaGameInstance.h"
#include "SpartaPlayerController.h"
#include "SpartaCharacter.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
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
	FName GameplayLevel("MenuLevel");
	if (UGameplayStatics::GetCurrentLevelName(this) == GameplayLevel)
	{
		return;
	}

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

	++CurrentWaveIndex;

	// ������ ���̺��� ���� ���̺꿡 ���� ���� Row(��)�� �����ɴϴ�.
	static const FString ContextString(TEXT("WaveContext"));
	FWaveRow* CurRows = WaveDataTable->FindRow<FWaveRow>(FName(*FString::FromInt(CurrentWaveIndex)), ContextString);
	
	// ���� ���̺꿡 ���� ������ �ִ� ��� ���̺� ������ ȣ���մϴ�.
	if (CurRows)
	{
		// ���� ���̺꿡 ���� �ؾ��� ������ ������ ���̺�
		if (UDataTable* CurItemSpawnDataTable = CurRows->ItemSpawnDataTable)
		{
			//���� ���̺꿡 �����ؾ��ϴ� �������� ������ �����;���
			int32 CurTotalActorSpawn = CurRows->TotalItemSpawn;

			StartWaveSpawnItem(CurTotalActorSpawn, CurItemSpawnDataTable);
		}

		// ���� ���̺꿡 ���� �ؾ��� ��ֹ� ������ ���̺�
		if (UDataTable* CurObstacleSpawnDataTable = CurRows->ObstacleSpawnDataTable)
		{
			int32 CurTotalActorSpawn = CurRows->TotalObstacleSpawn;
			StartWaveSpawnObstacle(CurTotalActorSpawn, CurObstacleSpawnDataTable);
		}

		// Ư�� �ð� �Ŀ� WaveCheck()�� ȣ��ǵ��� Ÿ�̸� ����
		GetWorldTimerManager().SetTimer(WavelTimerHandle, this, &ASpartaGameState::WaveCheck, LevelDuration, false);
	}
	// ���� ���̺꿡 ���� ������ ���� ��� ���� ������ ȣ���մϴ�.
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

void ASpartaGameState::StartWaveSpawnItem(const int32 TotalItemSpawn, const UDataTable* ItemSpawnDataTable)
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

	if (!FoundVolumes.Num())
	{
		return;
	}

	for (int32 i = 0; i < TotalItemSpawn; i++)
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

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Wave %d Start!"), CurrentWaveIndex));
}

void ASpartaGameState::StartWaveSpawnObstacle(const int32 ObstacleToSpawn, const UDataTable* ObstacleSpawnDataTable)
{
	// ���� �ʿ� ��ġ�� ��� SpawnVolume�� ã�� �������� ����
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	if (!FoundVolumes.Num())
	{
		return;
	}

	for (int32 i = 0; i < ObstacleToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomObstacle(ObstacleSpawnDataTable);
			}
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Obstacle Spawn!"), CurrentWaveIndex));
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

				if (UTextBlock* LevelText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					LevelText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex)));
				}

				if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(SpartaPlayerController->GetCharacter()))
				{
					float CurHealth = SpartaCharacter->GetHealth();
					float MaxHealth = SpartaCharacter->GetMaxHealth();

					if (UUserWidget* StatusWidget = Cast<UUserWidget>(HUDWidget->GetWidgetFromName(TEXT("WBP_Status"))))
					{
						if (UUserWidget* HPBarWidget = Cast<UUserWidget>(StatusWidget->GetWidgetFromName(TEXT("WBP_HPBar"))))
						{
							if (UImage* CurHPImage = Cast<UImage>(HPBarWidget->GetWidgetFromName(TEXT("CurHP"))))
							{
								FVector2D HPImageScale(CurHealth / MaxHealth, 1.f);
								CurHPImage->SetRenderScale(HPImageScale);
							}
						}

						if (UTextBlock* HPText = Cast<UTextBlock>(StatusWidget->GetWidgetFromName(TEXT("HPText"))))
						{
							HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), CurHealth, MaxHealth)));
						}
					}
				}
			}
		}
	}
}
