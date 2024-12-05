// This is Sandbox Project.

#include "SandboxProjectAnimInstance.h"
#include "SandboxProjectCharacter.h"

USandboxProjectAnimInstance::USandboxProjectAnimInstance(const FObjectInitializer& ObjectInitializer) 
{
	// 
}

void USandboxProjectAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	RefCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (RefCharacter)
	{
		RefCharacterMovement = Cast<UCharacterMovementComponent>(RefCharacter->GetCharacterMovement());
	}
}

void USandboxProjectAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!RefCharacter || !RefCharacterMovement) return;

	Velocity = RefCharacterMovement->Velocity;
	bIsFalling = RefCharacterMovement->IsFalling();
}

void USandboxProjectAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);
	if (!RefCharacter || !RefCharacterMovement) return;

	GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
	CurrentAcceleration = RefCharacterMovement->GetCurrentAcceleration();
	bIsMove = (GroundSpeed > 3.0f || ! UKismetMathLibrary::Vector_IsNearlyZero(Velocity));
}

void USandboxProjectAnimInstance::NativeBeginPlay()
{
	//
}
