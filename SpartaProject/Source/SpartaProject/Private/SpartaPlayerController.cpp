// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaPlayerController.h"
// Enhanced Input System�� Local Player Subsystem�� ����ϱ� ���� ����
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Blueprint/UserWidget.h"
#include "SpartaGameState.h"
#include "Components/TextBlock.h"
#include "SpartaGameInstance.h"
#include "Kismet/GameplayStatics.h"

ASpartaPlayerController::ASpartaPlayerController()
{
    InputMappingContext = nullptr;
    MoveAction = nullptr;
    JumpAction = nullptr;
    LookAction = nullptr;
    SprintAction = nullptr;
    HUDWidgetClass = nullptr;
    HUDWidgetInstance = nullptr;
    MainMenuWidgetClass = nullptr;
    MainMenuWidgetInstance = nullptr;
}

void ASpartaPlayerController::BeginPlay()
{
	Super::BeginPlay();

    // ���� PlayerController�� ����� Local Player ��ü�� ������
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    if (LocalPlayer)
    {
        // Local Player���� EnhancedInputLocalPlayerSubsystem�� ȹ��
        UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
        if (Subsystem)
        {
            if (!InputMappingContext.IsNull())
            {
                // Subsystem�� ���� �츮�� �Ҵ��� IMC�� Ȱ��ȭ
                // �켱����(Priority)�� 0�� ���� ���� �켱����
                Subsystem->AddMappingContext(InputMappingContext.LoadSynchronous(), 0);
            }
        }
    }

    FString CurrentMapName = GetWorld()->GetMapName();
    if (CurrentMapName.Contains("MenuLevel"))
    {
        ShowMainMenu(false);
    }
}

UUserWidget* ASpartaPlayerController::GetHUDWidget() const
{
    return HUDWidgetInstance;
}

// ���� HUD ǥ��
void ASpartaPlayerController::ShowGameHUD()
{
    // HUD�� ���� �ִٸ� �ݱ�
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    // �̹� �޴��� �� ������ ����
    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if (HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            SetShowMouseCursor(false);
            SetInputMode(FInputModeGameOnly());

            ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
            if (SpartaGameState)
            {
                SpartaGameState->UpdateHUD();
            }
        }
    }
}

void ASpartaPlayerController::ShowMainMenu(bool bIsRestart)
{
    if (HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if (MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if (MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if (MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            SetShowMouseCursor(true);
            SetInputMode(FInputModeUIOnly());
        }

        if (UTextBlock* ButtonText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
        {
            if (bIsRestart)
            {
                ButtonText->SetText(FText::FromString(TEXT("Restart")));

            }
            else
            {
                ButtonText->SetText(FText::FromString(TEXT("Start")));
            }
        }

        if (bIsRestart)
        {
            UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
            if (PlayAnimFunc)
            {
                MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
            }

            if (UTextBlock* TotalScoreText = Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
            {
                if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
                {
                    TotalScoreText->SetText(FText::FromString(FString::Printf(TEXT("Total Score: %d"), SpartaGameInstance->TotalScore)));
                }
            }
        }
    }
}

void ASpartaPlayerController::StartGame()
{
    if (USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        SpartaGameInstance->TotalScore = 0;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
    SetPause(false);
}
