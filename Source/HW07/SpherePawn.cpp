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


	// Ãß°¡
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

	if (!MovementInput.IsNearlyZero())
	{
		FVector MoveDirection = FVector(MovementInput.X, MovementInput.Y, 0.0f);

		AddActorLocalOffset(MoveDirection * MoveSpeed * DeltaTime, true);
	}

}

// Called to bind functionality to input
void ASpherePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASpherePawn::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASpherePawn::Look);
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

