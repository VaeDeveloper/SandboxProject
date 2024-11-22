// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "SandboxProjectAnimInstance.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), meta = (PrioritizeCategories = "Animation Instance"))
class SANDBOXPROJECT_API USandboxProjectAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	USandboxProjectAnimInstance(const FObjectInitializer& ObjectInitializer);
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeBeginPlay() override;

protected:
	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation Instance | Reference")
	TObjectPtr<ACharacter> RefCharacter = nullptr;

	UPROPERTY(Transient, BlueprintReadOnly, Category = "Animation Instance | Reference")
	TObjectPtr<UCharacterMovementComponent> RefCharacterMovement = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Instance | Anim Params")
	FVector Velocity = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Instance | Anim Params")
	FVector CurrentAcceleration = FVector::ZeroVector;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Instance | Anim Params")
	float GroundSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Instance | Anim Params")
	bool bIsMove = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Instance | Anim Params")
	bool bIsFalling = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation Instance | Anim Params")
	bool bIsRifle = false;

};
