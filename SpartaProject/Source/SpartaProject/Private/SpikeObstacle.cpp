// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeObstacle.h"
#include "Kismet/GameplayStatics.h"

ASpikeObstacle::ASpikeObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	// �޽ð� ���ʿ��ϰ� �浹�� ���� �ʵ��� �ϱ� ����, ������ NoCollision���� ����
	StaticMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	// Overlap �̺�Ʈ ���ε�
	StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &ASpikeObstacle::OnBeginOverlap);

	SpikeDamage = 10.f;
}

void ASpikeObstacle::BeginPlay()
{
	Super::BeginPlay();


}

void ASpikeObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ������ ���۵� ���� �� �ð�
	float Time = GetWorld()->GetTimeSeconds();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = LocationRange * FMath::Sin(PI * 2 * Time * LocationSpeed);

	SetActorLocation(CurrentLocation);
}

void ASpikeObstacle::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// ������
		UGameplayStatics::ApplyDamage(
			OtherActor,                      // �������� ���� ����
			SpikeDamage,            // ������ ��
			nullptr,                    // �������� ������ ��ü
			this,                       // �������� ������ ������Ʈ
			UDamageType::StaticClass());// �⺻ ������ ����
	}
}
