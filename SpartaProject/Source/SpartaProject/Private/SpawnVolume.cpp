// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    // �ڽ� ������Ʈ�� �����ϰ�, �� ������ ��Ʈ�� ����
    Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
    SetRootComponent(Scene);

    SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
    SpawningBox->SetupAttachment(Scene);
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
    // 1) �ڽ� ������Ʈ�� �����ϵ� Extent, �� x/y/z �������� ������(���� ����)�� ����
    FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
    FVector BoxOrigin = SpawningBox->GetComponentLocation();

    FVector Pointlocation = BoxOrigin + FVector(
        FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
        FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
        FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z));

    return Pointlocation;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass) return nullptr;

    AActor* SpawnActor = GetWorld()->SpawnActor<AActor>(ItemClass, GetRandomPointInVolume(), FRotator::ZeroRotator);

    return SpawnActor;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem(const UDataTable* ItemSpawnDataTable) const
{
    if (!ItemSpawnDataTable) return nullptr;

    // ��� Row(��) ��������
    TArray<FItemSpawnRow*> AllRows;
    static const FString ContextString(TEXT("ItemSpawnContext"));
    ItemSpawnDataTable->GetAllRows(ContextString, AllRows);

    if (AllRows.IsEmpty()) return nullptr;

    // ��ü Ȯ�� �� ���ϱ�
    float TotalChance = 0.0f;
    for (const FItemSpawnRow* Row : AllRows)
    {
        // Row�� ��ȿ�� ���
        if (Row)
        {
            TotalChance += Row->SpawnChance; // SpawnChance ���� TotalChance�� ���ϱ�
        }
    }

    // 0 ~ TotalChance ���� ���� ��
    const float RandValue = FMath::FRandRange(0.0f, TotalChance);
    float AccumulateChance = 0.0f;

    // ���� Ȯ���� ������ ����

    for (FItemSpawnRow* Row : AllRows)
    {
        AccumulateChance += Row->SpawnChance;
        if (RandValue <= AccumulateChance)
        {
            return Row;
        }
    }

    return nullptr;
}

AActor* ASpawnVolume::SpawnRandomItem(const UDataTable* ItemSpawnDataTable)
{
    if (FItemSpawnRow* SelectedRow = GetRandomItem(ItemSpawnDataTable))
    {
        if (UClass* ActualClass = SelectedRow->ItemClass.Get())
        {
            return SpawnItem(ActualClass);
        }
    }

    return nullptr;
}

