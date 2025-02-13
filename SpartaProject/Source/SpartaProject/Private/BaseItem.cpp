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

    // ��Ʈ ������Ʈ ���� �� ����
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    // �浹 ������Ʈ ���� �� ����
    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    // ��ħ�� �����ϴ� �������� ����
    Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
    // ��Ʈ ������Ʈ�� ����
    Collision->SetupAttachment(Scene);

    // ����ƽ �޽� ������Ʈ ���� �� ����
    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMesh->SetupAttachment(Collision);
    // �޽ð� ���ʿ��ϰ� �浹�� ���� �ʵ��� �ϱ� ����, ������ NoCollision���� ����
    StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

    // Overlap �̺�Ʈ ���ε�
    Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseItem::OnBeginOverlap);
    Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseItem::OnEndOverlap);

    // �⺻ ȸ�� �ӵ� (1�ʿ� 90�� ȸ��)
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

    // �ʴ� RotationSpeed��ŭ, �� �����Ӵ� (RotationSpeed * DeltaTime)��ŭ ȸ��
    AddActorLocalRotation(FRotator(0.f, RotationSpeed * DeltaTime, 0.f));
}

void ABaseItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag("Player"))
    {
        // ������ ��� (ȹ��) ���� ȣ��
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
