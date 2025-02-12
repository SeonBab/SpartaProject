// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeObstacle.h"
#include "Kismet/GameplayStatics.h"

ASpikeObstacle::ASpikeObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision으로 설정
	StaticMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	// Overlap 이벤트 바인딩
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

	// 게임이 시작된 후의 총 시간
	float Time = GetWorld()->GetTimeSeconds();

	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.Z = LocationRange * FMath::Sin(PI * 2 * Time * LocationSpeed);

	SetActorLocation(CurrentLocation);
}

void ASpikeObstacle::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// 데미지
		UGameplayStatics::ApplyDamage(
			OtherActor,                      // 데미지를 받을 액터
			SpikeDamage,            // 데미지 양
			nullptr,                    // 데미지를 유발한 주체
			this,                       // 데미지를 유발한 오브젝트
			UDamageType::StaticClass());// 기본 데미지 유형
	}
}
