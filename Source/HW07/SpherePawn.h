// SpherePawn.h

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"

#include "SpherePawn.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class HW07_API ASpherePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpherePawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	// IA 액션 기능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* UpDownAction;


	// 마우스 회전
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* PitchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* RollAction;

	float PitchInput = 0.0f;
	float RollInput = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float RotationSpeed = 100.0f;


	// 이동 기능
	FVector2D MovementInput;

	float UpDownInput = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MoveSpeed = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float LookSensitivity = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float VerticalMoveSpeed = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float AirControl = 0.5f;

	bool bIsGrounded = false;



	// 중력 기능
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gravity")
	float Gravity = -100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gravity")
	float VerticalVelocity = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gravity")
	bool bUseGravity = true;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	void UpDown(const FInputActionValue& value);

	void Pitch(const FInputActionValue& value);
	void Roll(const FInputActionValue& value);

};
