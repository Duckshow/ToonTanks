// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "Camera/CameraComponent.h"
#include "Debug/DebugDrawComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank() 
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetController());
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerController)
	{
		FHitResult HitResult;

		PlayerController->GetHitResultUnderCursor(
			ECollisionChannel::ECC_Visibility, 
			false, 
			HitResult);

		DrawDebugSphere(
			GetWorld(), 
			HitResult.ImpactPoint,
			25.f,
			12,
			FColor::Red);

		RotateTurret(HitResult.ImpactPoint);
	}
}

void ATank::HandleDestruction() 
{
	Super::HandleDestruction();
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	bAlive = false;
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATank::Move);
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(TurnAction, ETriggerEvent::Triggered, this, &ATank::Turn);
	}

	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(FireAction, ETriggerEvent::Completed, this, &ATank::HandleFireInput);
	}
}

void ATank::Move(const FInputActionValue& Value) 
{
	float MoveValue = Value.Get<float>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FVector DeltaLocation(MoveValue, 0.f, 0.f);
	DeltaLocation *= Speed * DeltaTime;

	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(const FInputActionValue& Value) 
{
	float TurnValue = Value.Get<float>();
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	FRotator DeltaRotation = FRotator(0.f, TurnValue, 0.f);
	DeltaRotation *= TurnRate * DeltaTime;

	AddActorLocalRotation(DeltaRotation, true);
}

void ATank::HandleFireInput(const FInputActionValue& Value) 
{
	Fire();
}