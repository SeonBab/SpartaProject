// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

AItem::AItem()
{
	// Scene Component를 생성하고 루트로 설정
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	// Static Mesh Component를 생성하고 Scene Component에 Attach
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	// Tick 함수를 켜줍니다.
	PrimaryActorTick.bCanEverTick = true;

	// 기본 회전 속도 (1초에 90도 회전)
	RotationSpeed = 90.f;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 기본 이동 속도 (1초에 200유닛 이동)
	float MoveSpeedZ = 200.f;

	// 초당 MoveSpeedZ만큼, 한 프레임당 (MoveSpeedZ * DeltaTime)만큼 Z축으로 이동
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += MoveSpeedZ * DeltaTime;

	// 위치 값 변경
	SetActorLocation(NewLocation);

	// 초당 RotationSpeed만큼, 한 프레임당 (RotationSpeed * DeltaTime)만큼 회전
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

	// 시작 기준이 되는 크기
	float BaseScale = 1.f;
	// 변경되는 스케일 값 ( -1 ~ 1 * 0.5f)
	float ScaleRange = 0.5f;
	// 스케일 변경 속도 (2초 마다)
	float ScaleSpeed = 0.5f;
	// 게임이 시작된 후의 총 시간
	float Time = GetWorld()->GetTimeSeconds();

	// 초당 ScaleSpeed만큼, 한 프레임당 ScaleRange * FMath::Sin(PI * 2 * Time * ScaleSpeed)만큼 스케일 변경
	FVector NewScale = FVector(BaseScale + ScaleRange * FMath::Sin(PI * 2 * Time * ScaleSpeed));

	SetActorScale3D(NewScale);
}

void AItem::ResetActorPosition()
{
	// (0, 0, 0) 위치로 되돌립니다.
	SetActorLocation(FVector::ZeroVector);
}

float AItem::GetRotationSpeed() const
{
	return RotationSpeed;
}

