// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseItem::ABaseItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // 루트 컴포넌트 생성 및 설정
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // 충돌 컴포넌트 생성 및 설정
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // 겹침만 감지하는 프로파일 설정
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // 루트 컴포넌트로 설정
    Collision->SetupAttachment(Scene);

    // 스태틱 메시 컴포넌트 생성 및 설정
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
    // 메시가 불필요하게 충돌을 막지 않도록 하기 위해, 별도로 NoCollision으로 설정
    StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

    // Overlap 이벤트 바인딩
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnBeginOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnEndOverlap);

    // 기본 회전 속도 (1초에 90도 회전)
    RotationSpeed = 90.f;
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 초당 RotationSpeed만큼, 한 프레임당 (RotationSpeed * DeltaTime)만큼 회전
    AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void ABaseItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        // 아이템 사용 (획득) 로직 호출
        ActivateItem(OtherActor);
    }
}

void ABaseItem::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void ABaseItem::ActivateItem(AActor* Activator)
{
    UParticleSystemComponent* Particle = nullptr;

    if (PickupParticle)
    {
        Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickupParticle, GetActorLocation(), GetActorRotation(), true);
    }

    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickupSound, GetActorLocation());
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

FName ABaseItem::GetItemType() const
{
	return ItemType;
}

void ABaseItem::DestroyItem()
{
	Destroy();
}
