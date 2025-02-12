// Fill out your copyright notice in the Description page of Project Settings.


#include "ReverseControlItem.h"
#include "SpartaCharacter.h"
#include "Components/SphereComponent.h"
#include "SpartaPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"

AReverseControlItem::AReverseControlItem()
{
	PrimaryActorTick.bCanEverTick = true;

	CrearReverseControllDelay = 3.f;

	ActivateTime = 0.f;
}

void AReverseControlItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DynamicMaterial)
	{
		ActivateTime += DeltaTime / CrearReverseControllDelay;
		DynamicMaterial->SetScalarParameterValue(TEXT("Percent"), ActivateTime);
	}
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	if (GetWorldTimerManager().IsTimerActive(ClearReverseControllTimerHandle))
	{
		return;
	}

	Super::ActivateItem(Activator);

	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(Activator))
	{
		ActivateActor = Activator;
		SpartaCharacter->SetIsReverseMoveInput(true);

		GetWorldTimerManager().SetTimer(ClearReverseControllTimerHandle, this, &AReverseControlItem::CrearReverseControll, CrearReverseControllDelay, false);

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
											CurDebuffImage->SetBrushFromTexture(ReverseControllTexture);
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

void AReverseControlItem::CrearReverseControll()
{
	if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(ActivateActor))
	{
		SpartaCharacter->SetIsReverseMoveInput(false);
	}

	CurDebuffUserWidget->SetVisibility(ESlateVisibility::Hidden);

	DestroyItem();
}
