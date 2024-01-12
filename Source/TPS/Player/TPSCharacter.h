#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "TPSCharacter.generated.h"

UCLASS()
class TPS_API ATPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ATPSCharacter();
	UFUNCTION(BlueprintCallable)
	inline FVector2D GetMotionDirection() { return MotionDirection; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);
	void MoveRight(float Value);

	void CalculateRotation(float DeltaTime);
	void CalculateMotion();

protected:
	UPROPERTY(EditAnywhere, Category = Camera) UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, Category = Camera) class USpringArmComponent* CameraBoom;
	UPROPERTY(EditAnywhere, Category = Movement) float BaseTurnRate = 2.0f;
	UPROPERTY(EditAnywhere, Category = Fire) FVector GunOffset;


private:
	class APController* PController;
	FVector2D InputDirection;
	FVector2D MotionDirection;
};
