// This is Sandbox Project.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NativeDebugExFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ESceneDepthPriority : uint8
{
	ESDP_Low	UMETA(DisplayName = "Low"),
	ESDP_Medium UMETA(DisplayName = "Medium"),
	ESDP_High	UMETA(DisplayName = "High")
};

class FExtendedDebugtHelpers : public UObject
{
public:
	static FCollisionQueryParams ConfigureCollisionParamsEx(FName TraceTag, bool bTraceComplex, const TArray<AActor*>& ActorsToIgnore, bool bIgnoreSelf, const UObject* WorldContextObject);
	static FCollisionObjectQueryParams ConfigureCollisionObjectParamsEx(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes);

	static void DrawDebugLineTraceSingleEx(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const FHitResult& HitResult, FLinearColor TraceColor, FLinearColor TraceHitColor, float Thickness,
										   float PointSize, float DrawTime);
	static void DrawDebugLineTraceMultiEx(const UWorld* World, const FVector& Start, const FVector& End, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const TArray<FHitResult>& HitResults, FLinearColor TraceColor, FLinearColor TraceHitColor, float Thickness,
										  float PointSize, float DrawTime);
	static void DrawDebugBoxTraceSingleEx(const UWorld* World, const FVector& Start, const FVector& End, const FVector& HalfSize, const FRotator& Orientation, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const FHitResult& HitResult, FLinearColor TraceColor,
										  FLinearColor TraceHitColor, float PointSize, float DrawTime);
	static void DrawDebugBoxTraceMultiEx(const UWorld* World, const FVector& Start, const FVector& End, const FVector& HalfSize, const FRotator& Orientation, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const TArray<FHitResult>& HitResults,
										 FLinearColor TraceColor, FLinearColor TraceHitColor, float PointSize, float DrawTime);
	static void DrawDebugSphereTraceSingleEx(const UWorld* World, const FVector& Start, const FVector& End, float Radius, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const FHitResult& HitResult, FLinearColor TraceColor, FLinearColor TraceHitColor,
											 float PointSize, float DrawTime);
	static void DrawDebugSphereTraceMultiEx(const UWorld* World, const FVector& Start, const FVector& End, float Radius, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const TArray<FHitResult>& HitResults, FLinearColor TraceColor, FLinearColor TraceHitColor,
											float PointSize, float DrawTime);
	static void DrawDebugCapsuleTraceSingleEx(const UWorld* World, const FVector& Start, const FVector& End, float Radius, float HalfHeight, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const FHitResult& HitResult, FLinearColor TraceColor,
											  FLinearColor TraceHitColor, float PointSize, float DrawTime);
	static void DrawDebugCapsuleTraceMultiEx(const UWorld* World, const FVector& Start, const FVector& End, float Radius, float HalfHeight, EDrawDebugTrace::Type DrawDebugType, ESceneDepthPriority DepthPriority, bool bHit, const TArray<FHitResult>& HitResults, FLinearColor TraceColor,
											 FLinearColor TraceHitColor, float PointSize, float DrawTime);
};

/**
 *
 */
UCLASS()
class NATIVEDEBUGMODULE_API UNativeDebugExFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
