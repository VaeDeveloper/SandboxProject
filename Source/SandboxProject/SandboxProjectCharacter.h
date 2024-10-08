// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "SandboxProjectCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UDelegateComponent;
class UThreadComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game, meta = (PrioritizeCategories = "PlayerCharacter"))
class ASandboxProjectCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ASandboxProjectCharacter();

	UFUNCTION()
	void SortComparatorActors(TSoftObjectPtr<AActor> FirstActor, TSoftObjectPtr<AActor> SecondActor, bool& Result);

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Getter")
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Getter")
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerCharacter|LevelActors")
	TArray<TSoftObjectPtr<AActor>> LevelActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

protected:
	virtual void BeginPlay();
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void TestDelegateBind();


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerCharacter|Delegates")
	TSubclassOf<AActor> SpawnActor;	

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerCharacter|Delegates")
	TObjectPtr<UDelegateComponent> DelegateComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Thread")
	TObjectPtr<UThreadComponent> ThreadComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Thread")
	int32 TotalActors = 10; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PlayerCharacter|Thread")
	int32 ActorsPerBatch = 1;

	UFUNCTION()
	void ArrayTestDelegate_Event(const TArray<int>& IntArray, const FString Str);

	FSimpleMulticastDelegate TestMulticastDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
};

