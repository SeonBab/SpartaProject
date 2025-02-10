// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SpartaPlayerController.generated.h"

// ���� ����
class UInputMappingContext;
class UInputAction;

UCLASS()
class SPARTAPROJECT_API ASpartaPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ASpartaPlayerController();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "HUD")
	UUserWidget* GetHUDWidget() const;

	// HUD ǥ��
	UFUNCTION(BlueprintCallable, Category = "HUD")
	void ShowGameHUD();
	// ���� �޴� ǥ��
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void ShowMainMenu(bool bIsRestart);
	// ���� ����
	UFUNCTION(BlueprintCallable, Category = "Menu")
	void StartGame();

	// �����Ϳ��� ������ IMC
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMappingContext;
	// IA_Move�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputAction> MoveAction;
	// IA_Jump�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputAction> JumpAction;
	// IA_Look�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputAction> LookAction;
	// IA_Sprint�� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	TSoftObjectPtr<UInputAction> SprintAction;

	// HUD UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	// �޴� UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
};
