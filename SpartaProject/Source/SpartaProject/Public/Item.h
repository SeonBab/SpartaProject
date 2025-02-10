// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class SPARTAPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �Լ��� �������Ʈ���� ȣ�� �����ϵ��� ����
	UFUNCTION(BlueprintCallable, Category = "Item|Actions")
	void ResetActorPosition();

	// �������Ʈ���� ���� ��ȯ�ϵ��� ����
	UFUNCTION(BlueprintPure, Category = "Item|Properties")
	float GetRotationSpeed() const;

	// C++���� ȣ������� ������ �������Ʈ���� ����
	UFUNCTION(BlueprintImplementableEvent, Category = "Item|Event")
	void OnItemPickedUp();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USceneComponent* SceneRoot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Components")
	UStaticMeshComponent* StaticMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item|Properties")
	float RotationSpeed;
};
