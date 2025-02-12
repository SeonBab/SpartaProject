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

	// 3�ʵ��� ExplosionDecalSize�� Ŀ����.
	float NewDecalSize = (ExplosionDecalSize / ExplosionDelay) * DeltaTime;

	// ���� �׷������� ��Į ũ�⺸�� Ŀ���ٸ� 0���� �ʱ�ȭ �� ���߷��� ����
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
			// ������
			UGameplayStatics::ApplyDamage(
				Actor,                      // �������� ���� ����
				ExplosionDamage,            // ������ ��
				nullptr,                    // �������� ������ ��ü (���ڸ� ��ġ�� ĳ���Ͱ� �����Ƿ� nullptr)
				this,                       // �������� ������ ������Ʈ(����)
				UDamageType::StaticClass());// �⺻ ������ ����
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
