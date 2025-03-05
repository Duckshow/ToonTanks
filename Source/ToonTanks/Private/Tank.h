// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "Tank.generated.h"

UCLASS()
class ATank : public ABasePawn
{
	GENERATED_BODY()

public:

	ATank();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	void HandleDestruction();
	APlayerController* GetPlayerController() const { return PlayerController; }

	bool bAlive = true;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* TurnAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	float Speed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Behavior")
	float TurnRate = 100.f;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Move(const struct FInputActionValue& Value);
	void Turn(const struct FInputActionValue& Value);
	void HandleFireInput(const struct FInputActionValue& Value);

	APlayerController* PlayerController;
};
