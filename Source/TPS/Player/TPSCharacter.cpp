#include "TPSCharacter.h"
#include "PController.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include <Kismet/KismetMathLibrary.h>
#include "DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"

ATPSCharacter::ATPSCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->TargetArmLength = 1200.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	PController = Cast<APController>(GetController());
	check(PController);
}

void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CalculateRotation(DeltaTime);
	CalculateMotion();

	if (InputDirection.Size() > 0)
		GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("InputDirection {%f,%f}"), InputDirection.X, InputDirection.Y));
}

void ATPSCharacter::CalculateRotation(float DeltaTime)
{
	if (PController)
	{
		FHitResult HitResult;
		if (PController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldStatic, true, HitResult))
		{
			//Rotation
			FRotator CurrentRotation = GetActorRotation();
			FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location);
			FRotator NewRotation = FRotator(CurrentRotation.Pitch, LookAtRotation.Yaw, CurrentRotation.Roll);
		
			//FQuat Result = FQuat::Slerp(CurrentRotation.Quaternion(), NewRotation.Quaternion(), DeltaTime);
			SetActorRotation(NewRotation, ETeleportType::None);
			//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), FVector(HitResult.Location.X, HitResult.Location.Y, GetActorLocation().Z), 10, FColor::Green, false, -1.0f, 0, 5.0f);
		}
	}
}
void ATPSCharacter::CalculateMotion()
{
	//Animation Motion calculation
	float forwardDirection = FVector::DotProduct(GetActorForwardVector(), FVector(InputDirection.X, InputDirection.Y, 0));
	float rightDirection = FVector::DotProduct(GetActorRightVector(), FVector(InputDirection.X, InputDirection.Y, 0));
	MotionDirection = FVector2D(forwardDirection, rightDirection);
}

void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::MoveRight);
}

void ATPSCharacter::MoveForward(float Value)
{
	InputDirection.Y = Value;
	if (Value != 0.0f) { AddMovementInput(Camera->GetForwardVector(), Value); }
}

void ATPSCharacter::MoveRight(float Value)
{
	InputDirection.X = Value;
	if (Value != 0.0f) { AddMovementInput(Camera->GetRightVector(), Value); }
}

