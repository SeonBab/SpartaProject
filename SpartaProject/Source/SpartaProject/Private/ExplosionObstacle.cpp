// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionObstacle.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

AExplosionObstacle::AExplosionObstacle()
{
	PrimaryActorTick.bCanEverTick = true;

	ExplosionDelay = 3.f;
	ExplosionRadius = 150.0f;
	ExplosionDamage = 10.0f;
	ExplosionDecalSize = 135.f;

	ExplosionCollision = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionCollision"));
	ExplosionCollision->InitSphereRadius(ExplosionRadius);
	ExplosionCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ExplosionCollision->SetupAttachment(Scene);

	ExplosionDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("ExplosionDecal"));
	ExplosionDecal->SetupAttachment(ExplosionCollision);
	ExplosionDecal->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	ExplosionDecal->DecalSize = FVector(512.f, ExplosionDecalSize, ExplosionDecalSize);
}

void AExplosionObstacle::BeginPlay()
{
	Super::BeginPlay();
}

void AExplosionObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 3초동안 ExplosionDecalSize로 커진다.
	float NewDecalSize = (ExplosionDecalSize / ExplosionDelay) * DeltaTime;

	// 만약 그려져야할 데칼 크기보다 커졌다면 0으로 초기화 후 폭발로직 실행
	FVector CurDecalSize = ExplosionDecal->DecalSize;
	CurDecalSize.Y += NewDecalSize;
	CurDecalSize.Z += NewDecalSize;

	if (CurDecalSize.Y > ExplosionDecalSize)
	{
		CurDecalSize.Y = 0;
		CurDecalSize.Z = 0;
		Explode();
	}

	ExplosionDecal->DecalSize = CurDecalSize;
	ExplosionDecal->MarkRenderStateDirty();
}

void AExplosionObstacle::Explode()
{
	UParticleSystemComponent* Particle = nullptr;

	if (ExplosionParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle, GetActorTransform(), false);
	}

	if (ExplosionSound)
	{
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, GetActorLocation());
	}

	TArray<AActor*> OverlappingActors;
	ExplosionCollision->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor->ActorHasTag("Player"))
		{
			// 데미지
			UGameplayStatics::ApplyDamage(
				Actor,                      // 데미지를 받을 액터
				ExplosionDamage,            // 데미지 양
				nullptr,                    // 데미지를 유발한 주체 (지뢰를 설치한 캐릭터가 없으므로 nullptr)
				this,                       // 데미지를 유발한 오브젝트(지뢰)
				UDamageType::StaticClass());// 기본 데미지 유형
		}
	}

	if (Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;

		GetWorld()->GetTimerManager().SetTimer(
			DestroyParticleTimerHandle,
			[Particle]()
			{
				if (!Particle->IsValidLowLevel()) return;
				Particle->DestroyComponent();
			},
			2.f,
			false
		);
	}
}
