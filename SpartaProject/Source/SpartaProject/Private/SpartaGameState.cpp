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
	LevelDuration = 10.0f; // 한 레벨당 30초
	CurrentWaveIndex = 0;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 첫 레벨부터 진행
	FName GameplayLevel("MenuLevel");
	if (UGameplayStatics::GetCurrentLevelName(this) == GameplayLevel)
	{
		return;
	}

	WaveCheck();

	// HUD에 대한 타이머 시작
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
	// 웨이브가 끝난 경우
	// 웨이브 수 초기화
	CurrentWaveIndex = 0;
	// 코인 개수 초기화
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

	// 데이터 테이블에서 현재 웨이브에 대한 정보 Row(행)을 가져옵니다.
	static const FString ContextString(TEXT("WaveContext"));
	FWaveRow* CurRows = WaveDataTable->FindRow<FWaveRow>(FName(*FString::FromInt(CurrentWaveIndex)), ContextString);
	
	// 현재 웨이브에 대한 정보가 있는 경우 웨이브 시작을 호출합니다.
	if (CurRows)
	{
		// 현재 웨이브에 스폰 해야할 아이템 데이터 테이블
		if (UDataTable* CurItemSpawnDataTable = CurRows->ItemSpawnDataTable)
		{
			//현재 웨이브에 스폰해야하는 아이템의 개수를 가져와야함
			int32 CurTotalActorSpawn = CurRows->TotalItemSpawn;

			StartWaveSpawnItem(CurTotalActorSpawn, CurItemSpawnDataTable);
		}

		// 현재 웨이브에 스폰 해야할 장애물 데이터 테이블
		if (UDataTable* CurObstacleSpawnDataTable = CurRows->ObstacleSpawnDataTable)
		{
			int32 CurTotalActorSpawn = CurRows->TotalObstacleSpawn;
			StartWaveSpawnObstacle(CurTotalActorSpawn, CurObstacleSpawnDataTable);
		}

		// 특정 시간 후에 WaveCheck()가 호출되도록 타이머 설정
		GetWorldTimerManager().SetTimer(WavelTimerHandle, this, &ASpartaGameState::WaveCheck, LevelDuration, false);
	}
	// 현재 웨이브에 대한 정보가 없는 경우 게임 오버를 호출합니다.
	else
	{
		// 타이머 해제
		GetWorldTimerManager().ClearTimer(WavelTimerHandle);

		OnGameOver();
	}
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
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

	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템을 스폰
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
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
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
	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템을 스폰
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
