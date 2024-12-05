// Copyright Epic Games, Inc. All Rights Reserved.

#include "SandboxProjectCharacter.h"
#include "SandboxProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "DelegateModule/Public/Components/DelegateComponent.h"
#include "ThreadsModule/Components/ThreadComponent.h"
#include "TagsModule/Public/TagComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);


void ASandboxProjectCharacter::TestDelegateBind()
{
	DelegateComponent->SimpleDel.BindLambda([]() -> void
		{
			UE_LOG(LogTemp, Warning, TEXT("Bind delegate"));
		});


	TestMulticastDelegate.AddLambda([]() -> void
		{
			UE_LOG(LogTemp, Warning, TEXT("Multicast delegate call"));
		});
}

ASandboxProjectCharacter::ASandboxProjectCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	DelegateComponent = CreateOptionalDefaultSubobject<UDelegateComponent>(TEXT("DelegateComponent"));
	check(DelegateComponent);
	
	ThreadComponent = CreateOptionalDefaultSubobject<UThreadComponent>(TEXT("ThreadComponent"));
	check(ThreadComponent);

	TagsComponent = CreateDefaultSubobject<UTagComponent>(TEXT("TagsComponent"));
	check(TagsComponent);


	TestDelegateBind();
}

void ASandboxProjectCharacter::SortComparatorActors(TSoftObjectPtr<AActor> FirstActor, TSoftObjectPtr<AActor> SecondActor, bool& Result)
{
	const FVector FActorLocation = FirstActor->GetActorLocation();
	const FVector SActorLocation = SecondActor->GetActorLocation();
	const FVector CharacterLocation = GetActorLocation();

	const float DistanceVectorResult = FVector::Distance(FActorLocation, CharacterLocation);
	const float DistanceVectorResultFromActor = FVector::Distance(SActorLocation, CharacterLocation);

	Result = DistanceVectorResult > DistanceVectorResultFromActor;
}

void ASandboxProjectCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	ThreadComponent->SpawnActorsAsync(GetWorld(), SpawnActor, TotalActors, ActorsPerBatch);

	//DelegateComponent->ArrayAndStringChangeDelegate.AddDynamic(this, &ASandboxProjectCharacter::ArrayTestDelegate_Event);

	//TArray<UObject*> SubObjectsList;
	//GetDefaultSubobjects(SubObjectsList);
	//for (const auto Sobj : SubObjectsList)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Found SubObjects FName = %s"), *Sobj->GetFName().ToString());
	//}
}

void ASandboxProjectCharacter::ArrayTestDelegate_Event(const TArray<int>& IntArray, const FString Str)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, Str);

	for (const auto i : IntArray)
	{
		const FString MSg = FString::Printf(TEXT("%i"), i);
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, MSg);
	}
}


void ASandboxProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASandboxProjectCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASandboxProjectCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ASandboxProjectCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}

}

void ASandboxProjectCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}