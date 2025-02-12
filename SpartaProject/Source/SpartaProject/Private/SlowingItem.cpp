// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingItem.h"
#include "SpartaCharacter.h"
#include "Components/SphereComponent.h"
#include "SpartaPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

ASlowingItem::ASlowingItem()
{
	PrimaryActorTick.bCanEverTick = true;

	ItemType = "Slow";

	ClearSlowDelay = 3.f;
	SlowSpeed = 0.5f;

	ActivateActor = nullptr;

	ActivateTime = 0.f;
}

void ASlowingItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DynamicMaterial)
	{
		ActivateTime += DeltaTime / ClearSlowDelay;
		DynamicMaterial->SetScalarParameterValue(TEXT("Percent"), ActivateTime);
	}
}

void ASlowingItem::ActivateItem(AActor* Activator)
{
	if (GetWorldTimerManager().IsTimerActive(ClearSlowTimerHandle))
	{
		return;
	}

	Super::ActivateItem(Activator);

	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Activator))
	{
		ActivateActor = Activator;
		SpartaCharacter->AddSlowSpeed(SlowSpeed);

		GetWorldTimerManager().SetTimer(ClearSlowTimerHandle, this, &ASlowingItem::ClearSlow, ClearSlowDelay, false);

		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
			{
				if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
				{
					if (UUserWidget* StatusWidget = Cast<UUserWidget>(HUDWidget->GetWidgetFromName(TEXT("WBP_Status"))))
					{
						if (UHorizontalBox* DebuffHorizontalBox = Cast<UHorizontalBox>(StatusWidget->GetWidgetFromName(TEXT("HorizontalBox_Debuff"))))
						{
							TArray<UWidget*> DebuffWidgetArray = DebuffHorizontalBox->GetAllChildren();

							for (auto DebuffWidget : DebuffWidgetArray)
							{
								if (UUserWidget* DebuffUserWidget = Cast<UUserWidget>(DebuffWidget))
								{
									if (ESlateVisibility::Hidden == DebuffUserWidget->GetVisibility())
									{
										CurDebuffUserWidget = DebuffUserWidget;
										DebuffUserWidget->SetVisibility(ESlateVisibility::Visible);

										if (UImage* CurDebuffImage = Cast<UImage>(DebuffUserWidget->GetWidgetFromName(TEXT("Debuff"))))
										{
											CurDebuffImage->SetBrushFromTexture(SlowTexture);
										}

										if (UImage* CurCoolDownImage = Cast<UImage>(DebuffUserWidget->GetWidgetFromName(TEXT("CoolDown"))))
										{
											DynamicMaterial = UMaterialInstanceDynamic::Create(CoolDownMaterialInstance, this);
											if (DynamicMaterial)
											{
												CurCoolDownImage->SetBrushFromMaterial(DynamicMaterial);
											}
										}

										break;
									}
								}
							}
						}
					}
				}
			}
		}

		// 보이지 않게 설정
		SetHidden(true);
		// 더이상 오버랩 되지 않도록 충돌 변경
		StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));
	}
}

void ASlowingItem::ClearSlow()
{
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(ActivateActor))
	{
		SpartaCharacter->RemoveSlowSpeed(SlowSpeed);
	}

	CurDebuffUserWidget->SetVisibility(ESlateVisibility::Hidden);

	DestroyItem();
}
