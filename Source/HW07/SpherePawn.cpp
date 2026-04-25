// SpherePawn.cpp

// Fill out your copyright notice in the Description page of Project Settings.


#include "SpherePawn.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"

// Sets default values
ASpherePawn::ASpherePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));


	// 추가
	// /Script/Engine.StaticMesh'/Engine/VREditor/BasicMeshes/SM_Ball_01.SM_Ball_01'
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyStaticMesh(TEXT("/Engine/VREditor/BasicMeshes/SM_Ball_01.SM_Ball_01"));
	if (BodyStaticMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(BodyStaticMesh.Object);
	}



	RootComponent = CollisionComponent;

	MeshComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(SpringArmComponent);

	CollisionComponent->InitSphereRadius(50.0f);

	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollisionComponent->SetSimulatePhysics(false);
	MeshComponent->SetSimulatePhysics(false);

	SpringArmComponent->TargetArmLength = 300.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void ASpherePawn::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void ASpherePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float CurrentMoveSpeed = bIsGrounded ? MoveSpeed : MoveSpeed * AirControl;

	FVector MoveDelta = FVector(
		MovementInput.X * CurrentMoveSpeed * DeltaTime,
		MovementInput.Y * CurrentMoveSpeed * DeltaTime,
		UpDownInput * CurrentMoveSpeed * DeltaTime
	);

	if (!MoveDelta.IsNearlyZero())
	{
		FHitResult InputHit;
		AddActorLocalOffset(MoveDelta, true, &InputHit);
	}

	// 중력/착지 판정
	if (bUseGravity)
	{
		float CurrentGravity = Gravity;

		if (UpDownInput > 0.0f)
		{
			CurrentGravity = Gravity * 0.3f;
		}

		VerticalVelocity += CurrentGravity * DeltaTime;

		FVector GravityDelta = FVector(0.0f, 0.0f, VerticalVelocity * DeltaTime);

		FHitResult GravityHit;
		AddActorWorldOffset(GravityDelta, true, &GravityHit);

		if (GravityHit.bBlockingHit && GravityHit.ImpactNormal.Z > 0.5f)
		{
			VerticalVelocity = 0.0f;
			bIsGrounded = true;
		}
		else
		{
			bIsGrounded = false;
		}
	}

	FRotator RotationDelta = FRotator(
		PitchInput * RotationSpeed * DeltaTime,
		0.0f,
		RollInput * RotationSpeed * DeltaTime
	);

	if (!RotationDelta.IsNearlyZero())
	{
		AddActorLocalRotation(RotationDelta);
	}
}

// Called to bind functionality to input
void ASpherePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpherePawn::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASpherePawn::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpherePawn::Look);

		EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Triggered, this, &ASpherePawn::UpDown);
		EnhancedInputComponent->BindAction(UpDownAction, ETriggerEvent::Completed, this, &ASpherePawn::UpDown);

		EnhancedInputComponent->BindAction(PitchAction, ETriggerEvent::Triggered, this, &ASpherePawn::Pitch);
		EnhancedInputComponent->BindAction(PitchAction, ETriggerEvent::Completed, this, &ASpherePawn::Pitch);

		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &ASpherePawn::Roll);
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Completed, this, &ASpherePawn::Roll);
	}

}

void ASpherePawn::Move(const FInputActionValue& value)
{
	MovementInput = value.Get<FVector2D>();

	UE_LOG(LogTemp, Warning, TEXT("Move Input: X=%f, Y=%f"), MovementInput.X, MovementInput.Y);
}

void ASpherePawn::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	float DeltaTime = GetWorld()->GetDeltaSeconds();

	AddActorLocalRotation(FRotator(0.0f, LookInput.X * LookSensitivity * DeltaTime, 0.0f));
}

void ASpherePawn::UpDown(const FInputActionValue& value)
{
	UpDownInput = value.Get<float>();
}

void ASpherePawn::Pitch(const FInputActionValue& value)
{
	PitchInput = value.Get<float>();
}

void ASpherePawn::Roll(const FInputActionValue& value)
{
	RollInput = value.Get<float>();
}

