// Fill out your copyright notice in the Description page of Project Settings.


#include "SpartaCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// �Է� ���ε忡 �ʿ��� ���
#include "EnhancedInputComponent.h"
// �÷��̾� ��Ʈ�ѷ� ���
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "Components/WidgetComponent.h"
#include "Components/Image.h"

ASpartaCharacter::ASpartaCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// ������ �� ����
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// ��Ʈ ������Ʈ�� ��Ʈ�� �� ����
	SpringArmComp->SetupAttachment(RootComponent);
	// ĳ���Ϳ� ī�޶� ������ �⺻ �Ÿ��� 300���� ����
	SpringArmComp->TargetArmLength = 300.f;
	// ��Ʈ�ѷ� ȸ���� ���� ������ �ϵ� ȸ���ϵ��� ����
	SpringArmComp->bUsePawnControlRotation = true;

	// ī�޶� ����
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// ������ ���� ���� ��ġ�� ī�޶� ����
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// ī�޶�� ������ ���� ȸ���� �����Ƿ� ȸ������ �ʵ��� ����
	CameraComp->bUsePawnControlRotation = false;

    OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
    OverheadWidget->SetupAttachment(GetMesh());
    OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

    NormalSpeed = 600.0f;
    SprintSpeedMultiplier = 1.5f;
    SprintSpeed = NormalSpeed * SprintSpeedMultiplier;

    bIsReverseMoveInput = false;

    GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

    // �ʱ� ü�� ����
    MaxHealth = 100.0f;
    Health = MaxHealth;
}

int32 ASpartaCharacter::GetHealth() const
{
    return Health;
}

void ASpartaCharacter::AddHealth(float Amount)
{
    Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);

    UpdateOverheadHP();
}

int32 ASpartaCharacter::GetMaxHealth() const
{
    return MaxHealth;
}

void ASpartaCharacter::AddSlowSpeed(float SlowSpeed)
{
    GetCharacterMovement()->MaxWalkSpeed *= SlowSpeed;
    NormalSpeed *= SlowSpeed;
    SprintSpeed *= SlowSpeed;
}

void ASpartaCharacter::RemoveSlowSpeed(float SlowSpeed)
{
    float Reciprocal = 1.f / SlowSpeed;
    GetCharacterMovement()->MaxWalkSpeed *= Reciprocal;
    NormalSpeed *= Reciprocal;
    SprintSpeed *= Reciprocal;
}

void ASpartaCharacter::SetIsReverseMoveInput(bool NewbIsReverseMoveInput)
{
    bIsReverseMoveInput = NewbIsReverseMoveInput;
}

void ASpartaCharacter::BeginPlay()
{
	Super::BeginPlay();
	
    UpdateOverheadHP();
}

void ASpartaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Enhanced InputComponent�� ĳ����
    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        // IA�� �������� ���� ���� ���� ���� Controller�� ASpartaPlayerController�� ĳ����
        if (ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetController()))
        {
            if (!PlayerController->MoveAction.IsNull())
            {
                // IA_Move �׼� Ű�� "Ű�� ������ �ִ� ����" Move() ȣ��
                EnhancedInput->BindAction(PlayerController->MoveAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ASpartaCharacter::Move);
            }

            if (!PlayerController->JumpAction.IsNull())
            {
                // IA_Jump �׼� Ű�� "Ű�� ������ �ִ� ����" StartJump() ȣ��
                EnhancedInput->BindAction(PlayerController->JumpAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ASpartaCharacter::StartJump);

                // IA_Jump �׼� Ű���� "���� �� ����" StopJump() ȣ��
                EnhancedInput->BindAction(PlayerController->JumpAction.LoadSynchronous(), ETriggerEvent::Completed, this, &ASpartaCharacter::StopJump);
            }

            if (!PlayerController->LookAction.IsNull())
            {
                // IA_Look �׼� ���콺�� "������ ��" Look() ȣ��
                EnhancedInput->BindAction(PlayerController->LookAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ASpartaCharacter::Look);
            }

            if (!PlayerController->SprintAction.IsNull())
            {
                // IA_Sprint �׼� Ű�� "������ �ִ� ����" StartSprint() ȣ��
                EnhancedInput->BindAction(PlayerController->SprintAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &ASpartaCharacter::StartSprint);

                // IA_Sprint �׼� Ű���� "���� �� ����" StopSprint() ȣ��
                EnhancedInput->BindAction(PlayerController->SprintAction.LoadSynchronous(), ETriggerEvent::Completed, this, &ASpartaCharacter::StopSprint);
            }
        }
    }
}

void ASpartaCharacter::Move(const FInputActionValue& Value)
{
    if (!Controller) return;

    // Value�� Axis2D�� ������ IA_Move�� �Է°� (WASD)�� ��� ����
    // ��) (X=1, Y=0) �� ���� / (X=-1, Y=0) �� ���� / (X=0, Y=1) �� ������ / (X=0, Y=-1) �� ����
    const FVector2D MoveInput = Value.Get<FVector2D>();

    if (MoveInput.X != 0.f)
    {
        float XValue = (bIsReverseMoveInput) ? MoveInput.X * -1 : MoveInput.X;
        // ĳ���Ͱ� �ٶ󺸴� ����(����)���� X�� �̵�
        AddMovementInput(GetActorForwardVector(), XValue);
    }

    if (MoveInput.Y != 0.f)
    {
        float YValue = (bIsReverseMoveInput) ? MoveInput.Y * -1 : MoveInput.Y;
        // ĳ������ ������ �������� Y�� �̵�
        AddMovementInput(GetActorRightVector(), YValue);
    }
}

void ASpartaCharacter::StartJump(const FInputActionValue& Value)
{
    // Jump �Լ��� Character�� �⺻ ����
    if (Value.Get<bool>())
    {
        Jump();
    }
}

void ASpartaCharacter::StopJump(const FInputActionValue& Value)
{
    // StopJumping �Լ��� Character�� �⺻ ����
    if (!Value.Get<bool>())
    {
        StopJumping();
    }
}

void ASpartaCharacter::Look(const FInputActionValue& Value)
{
    // ���콺�� X, Y �������� 2D ������ ������
    FVector2D LookInput = Value.Get<FVector2D>();

    // X�� �¿� ȸ�� (Yaw), Y�� ���� ȸ�� (Pitch)
    // �¿� ȸ��
    AddControllerYawInput(LookInput.X);
    // ���� ȸ��
    AddControllerPitchInput(LookInput.Y);
}

void ASpartaCharacter::StartSprint(const FInputActionValue& Value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
    }
}

void ASpartaCharacter::StopSprint(const FInputActionValue& Value)
{
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
    }
}

float ASpartaCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    Health = FMath::Clamp(Health - ActualDamage, 0.0f, MaxHealth);
    UpdateOverheadHP();

    if (Health <= 0.f)
    {
        OnDeath();
    }

    return ActualDamage;
}

void ASpartaCharacter::OnDeath()
{
    // ��� �� ����
    ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
    if (SpartaGameState)
    {
        SpartaGameState->OnGameOver();
    }
}

void ASpartaCharacter::UpdateOverheadHP()
{
    if (!OverheadWidget)
    {
        return;
    }

    UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
    if (OverheadWidgetInstance)
    {
        if (UImage* CurHPImage = Cast<UImage>(OverheadWidgetInstance->GetWidgetFromName(TEXT("CurHP"))))
        {
            FVector2D HPImageScale(Health / MaxHealth, 1.f);
            CurHPImage->SetRenderScale(HPImageScale);
        }
    }
}

