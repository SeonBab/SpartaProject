// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseObstacle.h"

ABaseObstacle::ABaseObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// ��Ʈ ������Ʈ ���� �� ����
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	// ����ƽ �޽� ������Ʈ ���� �� ����
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Scene);
}

void ABaseObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

