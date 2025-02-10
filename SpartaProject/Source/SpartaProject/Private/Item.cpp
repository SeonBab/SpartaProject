// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

AItem::AItem()
{
	// Scene Component�� �����ϰ� ��Ʈ�� ����
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	// Static Mesh Component�� �����ϰ� Scene Component�� Attach
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	// Tick �Լ��� ���ݴϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// �⺻ ȸ�� �ӵ� (1�ʿ� 90�� ȸ��)
	RotationSpeed = 90.f;
}

void AItem::BeginPlay()
{
	Super::BeginPlay();
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// �⺻ �̵� �ӵ� (1�ʿ� 200���� �̵�)
	float MoveSpeedZ = 200.f;

	// �ʴ� MoveSpeedZ��ŭ, �� �����Ӵ� (MoveSpeedZ * DeltaTime)��ŭ Z������ �̵�
	FVector NewLocation = GetActorLocation();
	NewLocation.Z += MoveSpeedZ * DeltaTime;

	// ��ġ �� ����
	SetActorLocation(NewLocation);

	// �ʴ� RotationSpeed��ŭ, �� �����Ӵ� (RotationSpeed * DeltaTime)��ŭ ȸ��
	AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));

	// ���� ������ �Ǵ� ũ��
	float BaseScale = 1.f;
	// ����Ǵ� ������ �� ( -1 ~ 1 * 0.5f)
	float ScaleRange = 0.5f;
	// ������ ���� �ӵ� (2�� ����)
	float ScaleSpeed = 0.5f;
	// ������ ���۵� ���� �� �ð�
	float Time = GetWorld()->GetTimeSeconds();

	// �ʴ� ScaleSpeed��ŭ, �� �����Ӵ� ScaleRange * FMath::Sin(PI * 2 * Time * ScaleSpeed)��ŭ ������ ����
	FVector NewScale = FVector(BaseScale + ScaleRange * FMath::Sin(PI * 2 * Time * ScaleSpeed));

	SetActorScale3D(NewScale);
}

void AItem::ResetActorPosition()
{
	// (0, 0, 0) ��ġ�� �ǵ����ϴ�.
	SetActorLocation(FVector::ZeroVector);
}

float AItem::GetRotationSpeed() const
{
	return RotationSpeed;
}

