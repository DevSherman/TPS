#include "PlayerAnimation.h"
#include "TPSCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimation::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	TPSCharacter = Cast<ATPSCharacter>(TryGetPawnOwner());
	check(TPSCharacter)
}

void UPlayerAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!TPSCharacter) return;

	MotionDirection = TPSCharacter->GetMotionDirection();
	Velocity = TPSCharacter->GetVelocity().Size();

	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("Velocity %f"), Velocity));
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Green, FString::Printf(TEXT("MotionDirection {%f,%f}"), MotionDirection.X, MotionDirection.Y));
}

