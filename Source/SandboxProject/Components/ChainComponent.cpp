// This is Sandbox Project. 


#include "ChainComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineGlobals.h"

UChainComponent::UChainComponent(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;
	InstanceComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstanceComponent"));
	InstanceComponent->InstancingRandomSeed = FMath::Rand();
}

void UChainComponent::OnRegister()
{
	Super::OnRegister();

	InitChain();
}

void UChainComponent::OnUnregister()
{
	Super::OnUnregister();

	InstanceComponent->ClearInstances();
	ChainPoints.Empty();
}

void UChainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Frame++;

	if (Frame > 0 && (Skip + 1) > 0)
	{
		if (Frame % (Skip + 1) == 0)
		{
			CalculatePoints();
		}
	}

	DrawChainPoints();
}

void UChainComponent::BeginPlay()
{
	Super::BeginPlay();

	if (HasBegunPlay())
	{
		InitChain();
	}
}

TArray<int> UChainComponent::ApplyForce(FVector InPosition, float InRadius, FVector InForce)
{
	TArray<int> AffectedPoints;
	for (auto& Point : ChainPoints)
	{
		if ((InPosition - Point.Position).Size() <= InRadius)
		{
			AffectedPoints.Add(Point.Index);
			Point.Force -= InForce;
		}
	}

	return AffectedPoints;
}

FVector UChainComponent::GetChainPoint(int index)
{
	return ChainPoints.Num() > index ? ChainPoints[index].Position : FVector::ZeroVector;
}

void UChainComponent::AttachStartToActor(FComponentReference ComponentReference, FName Socket)
{
	AttachStartTo = ComponentReference;
	AttachStartToSocket = Socket;
	AttachStart = true;
}

void UChainComponent::AttachEndToActor(FComponentReference ComponentReference, FName Socket)
{
	AttachEndTo = ComponentReference;
	AttachEndToSocket = Socket;
	AttachEnd = true;
}

void UChainComponent::InitChain()
{
	InstanceComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ChainPoints.Reset();
	ChainPoints.AddUninitialized(Segments);

	ChainStart = GetComponentLocation();
	ChainEnd = GetChainEndPoint();

	const FVector LengthVector = ChainEnd - ChainStart;
	InstanceComponent->SetStaticMesh(ChainMesh);
	InstanceComponent->ClearInstances();

	if (InstanceComponent->GetInstanceCount() == 0)
	{
		float SegmentTime = 1.0f / static_cast<float>(Segments);
		SegmentLength = (LengthVector.Size() / static_cast<float>(Segments)) * ChainLength;

		for (int i = 0; i < Segments; i++)
		{
			FChainPointData ChainPoint;
			ChainPoint.Time = i * SegmentTime;
			ChainPoint.Index = i;
			ChainPoint.Position = ChainStart + ((static_cast<float>(i) / static_cast<float>(Segments)) * LengthVector);
			ChainPoint.OldPosition = ChainPoint.Position;
			ChainPoint.Transform.SetScale3D(Scale);

			ChainPoints[i] = ChainPoint;
			InstanceComponent->AddInstance(ChainPoint.Transform);
		}
	}
}

void UChainComponent::DrawChainPoints()
{
#if WITH_EDITOR
	if (!bDrawDebugger || ChainPoints.Num() < 2) return;

	const int32 LineEndMultiply = 5;
	const int32 SphereSegments = 4;
	const int32 SphereWidthMultiply = 2;

	DrawDebugSphere(GetWorld(), ChainPoints[0].Position, ChainWidth * SphereWidthMultiply, SphereSegments, FColor::Green);

	for (int32 i = 1; i < ChainPoints.Num() - 1; i++)
	{
		DrawDebugSphere(GetWorld(), ChainPoints[i].Position, ChainWidth, SphereSegments, FColor::Red);
		FVector Forward = FVector::CrossProduct(ChainPoints[i].Direction, FVector::ForwardVector);
		FVector Right = FVector::CrossProduct(ChainPoints[i].Direction, Forward);
		DrawDebugLine(GetWorld(), ChainPoints[i].Position, ChainPoints[i].Position + ChainPoints[i].Direction * LineEndMultiply, FColor::Red);
		DrawDebugLine(GetWorld(), ChainPoints[i].Position, ChainPoints[i].Position + Forward * LineEndMultiply, FColor::Green);
		DrawDebugLine(GetWorld(), ChainPoints[i].Position, ChainPoints[i].Position + Right * LineEndMultiply, FColor::Blue);
	}

	DrawDebugSphere(GetWorld(), ChainPoints.Last().Position, ChainWidth * SphereWidthMultiply, SphereSegments, FColor::Green, false, -1, 0, 2);
#endif
}

void UChainComponent::UpdateAttachments()
{
	if (ChainPoints.Num() > 0)
	{
		if (AttachComponentToStart.OtherActor != nullptr)
		{
			USceneComponent* AttachToStart = Cast<USceneComponent>(AttachComponentToStart.GetComponent(GetOwner()));

			if (AttachToStart)
			{
				if (RotateStartAttachment)
				{
					AttachToStart->SetWorldLocationAndRotationNoPhysics(ChainPoints[0].Position, ChainPoints[0].Rotation);
				}
				else
				{
					AttachToStart->SetWorldLocation(ChainPoints[0].Position);
				}
			}
		}
		if (AttachComponentToEnd.OtherActor != nullptr)
		{
			USceneComponent* AttachToEnd = Cast<USceneComponent>(AttachComponentToEnd.GetComponent(GetOwner()));
			AttachToEnd->SetWorldLocationAndRotationNoPhysics(ChainPoints.Last().Position, ChainPoints.Last().Rotation);

			if (AttachToEnd)
			{
				if (bRotateEndAttachment)
				{
					AttachToEnd->SetWorldLocationAndRotationNoPhysics(ChainPoints[0].Position, ChainPoints[0].Rotation);
				}
				else
				{
					AttachToEnd->SetWorldLocation(ChainPoints[0].Position);
				}
			}
		}
	}
}

void UChainComponent::CalculatePoints()
{
	if (ChainPoints.Num() < 2) return;

	ChainEnd = GetChainEndPoint();

	CalculateChainPoint(AttachStart, AttachStartTo, AttachStartToSocket, 0);
	CalculateChainPoint(AttachEnd, AttachEndTo, AttachEndToSocket, ChainPoints.Num() - 1, true);

	ApplyGravity();
	SolveConstraint();
	ResolveCollision();
	UpdateMeshes();
	UpdateAttachments();
}

void UChainComponent::ApplyGravity()
{

	constexpr float GravityScale = 1000.0f;
	const FVector GravityVector(0, 0, GetWorld()->GetGravityZ() * Gravity / GravityScale);

	for (auto& Point : ChainPoints)
	{
		if (Point.bFree)
		{
			FVector Velocity = (Point.Position - Point.OldPosition) + GravityVector; 
			Point.OldPosition = Point.Position;
			Point.Position += Velocity; 
			Point.Velocity = Velocity;  
		}
	}
}

void UChainComponent::SolveConstraint()
{
	const int32 NumSegments = ChainPoints.Num() - 1;

	for (int32 i = 0; i < NumSegments; i++)
	{
		UpdatePoint(i, ChainPoints[i], ChainPoints[i + 1], SegmentLength, 1);
	}

	UpdatePoint(NumSegments, ChainPoints[NumSegments - 1], ChainPoints[NumSegments], SegmentLength, 1);

	for (int i = 0; i < Stiffness; i++)
	{
		for (int32 j = 0; j < NumSegments; j++)
		{
			UpdatePoint(j, ChainPoints[j], ChainPoints[j + 1], SegmentLength, 1); 
		}

		UpdatePoint(NumSegments, ChainPoints[NumSegments], ChainPoints[NumSegments - 1], SegmentLength, 1);
	}
}

void UChainComponent::ResolveCollision()
{
	UWorld* World = GetWorld();

	if (GetCollisionEnabled() != ECollisionEnabled::NoCollision)
	{
		FCollisionQueryParams Params(SCENE_QUERY_STAT(CableCollision));
		FCollisionResponseParams ResponseParam(GetCollisionResponseToChannels());
		FVector Velocity = FVector::ZeroVector;
		FVector Normal = FVector::ZeroVector;
		FVector Position = FVector::ZeroVector;

		for (int32 i = 0; i < ChainPoints.Num(); i++)
		{
			if (ChainPoints[i].bFree)
			{
				if (bSelfCollision)
				{
					Normal = FVector::ZeroVector;
					Position = ChainPoints[i].Position;

					for (int32 j = 0; j < ChainPoints.Num(); j++)
					{
						int d = j - i;
						if (i != j && abs(d) > 2)
						{
							const float dist = FVector::Dist(ChainPoints[i].Position, ChainPoints[j].Position);

							if (abs(dist) < SelfCollisionWidth)
							{
								Normal = ChainPoints[i].Position - ChainPoints[j].Position;
								Normal *= ((Normal.Size() - SelfCollisionWidth) / SelfCollisionWidth);
								if (!Normal.IsNearlyZero(SelfCollisionThreshold))
								{
									ChainPoints[i].Force += Normal;
								}
							}
						}
					}
				}

				TArray<FHitResult> HitResult;
				bool Hitted = World->SweepMultiByChannel(
					HitResult
					, ChainPoints[i].Position
					, ChainPoints[i].Position + ChainPoints[i].Velocity
					, FQuat::Identity
					, GetCollisionObjectType()
					, FCollisionShape::MakeSphere(0.5f * ChainWidth)
					, Params
					, ResponseParam
				);

				if (Hitted)
				{
					OnCollide.Broadcast(HitResult);

					for (int32 j = 0; j < HitResult.Num(); j++)
					{
						if (HitResult[j].bStartPenetrating) 
						{
							ChainPoints[i].Position += (HitResult[j].Normal * HitResult[j].PenetrationDepth);
						}
						else 
						{
							ChainPoints[i].Position = HitResult[j].Location;
						}

						FVector Delta = ChainPoints[i].Position - ChainPoints[i].OldPosition;
						float nDelta = Delta | HitResult[j].Normal;
						FVector plane = Delta - (nDelta * HitResult[j].Normal);
						ChainPoints[i].OldPosition += nDelta * HitResult[j].Normal;
						
						if (Friction > KINDA_SMALL_NUMBER)
						{
							ChainPoints[i].OldPosition += plane * Friction;
						}
					}
				}
			}
			Velocity += ChainPoints[i].Velocity;
		}

		if (Velocity.Size() > SoundThreshold && Frame % SoundSkip == 0)
		{
			OnSoundReached.Broadcast(Velocity);
		}
	}
}

void UChainComponent::UpdateMeshes()
{
	for (int32 i = 0; i < ChainPoints.Num(); i++)
	{
		FChainPointData& ChainPoint = ChainPoints[i]; 

		ChainPoint.Transform.SetLocation(ChainPoint.Position);
		ChainPoint.Transform.SetRotation(ChainPoint.Rotation.Quaternion());

		InstanceComponent->UpdateInstanceTransform(i, ChainPoint.Transform, true, true);
	}
}

void UChainComponent::UpdatePoint(size_t i, FChainPointData& p1, FChainPointData& p2, float length, int offset)
{
	const FVector Delta = p2.Position - p1.Position;
	if (Delta.IsNearlyZero()) return;

	const float CurrentDistance = Delta.Size();
	const float MaxDistance = (CurrentDistance - length) / CurrentDistance;

	if (p1.bFree && p2.bFree)
	{
		FVector Force = MaxDistance * 0.5f * Delta;
		p1.Position += Force + p1.Force;
		p2.Position -= Force + p2.Force;
		p1.Force = FVector::ZeroVector;
		p2.Force = FVector::ZeroVector;
	}
	else if (p1.bFree)
	{
		p1.Position += (MaxDistance * Delta) + p1.Force;
		p1.Force = FVector::ZeroVector;
	}
	else if (p2.bFree)
	{
		p2.Position -= (MaxDistance * Delta) + p2.Force;
		p2.Force = FVector::ZeroVector;
	}

	FVector Direction;

	if (i == 0)
	{
		Direction = (ChainPoints[i + 1].Position - p1.Position).GetUnsafeNormal();
	}
	else if (i == ChainPoints.Num() - 1)
	{
		Direction = (p1.Position - ChainPoints[i - 1].Position).GetUnsafeNormal();
	}
	else
	{
		Direction = (ChainPoints[i + 1].Position - ChainPoints[i - 1].Position).GetUnsafeNormal();
	}

	FVector Forward = FVector::CrossProduct(Direction, FVector::ForwardVector);
	FVector Right = FVector::CrossProduct(Direction, Forward);

	p1.Direction = Right;
	p1.Rotation = p1.Direction.ToOrientationRotator();

	p1.Rotation.Add(90 + AdditiveRotation.X * i, AdditiveRotation.Y * i, AdditiveRotation.Z * i);

}

FVector UChainComponent::GetChainEndPoint() const
{
	if (AttachEndTo.OtherActor == nullptr)
	{
		return bIsLocal ? GetComponentLocation() + EndPoint : EndPoint;
	}

	USceneComponent* EndComponent = Cast<USceneComponent>(AttachEndTo.GetComponent(GetOwner()));
	
	if (EndComponent)
	{
		return (AttachEndToSocket != NAME_None)
			? EndComponent->GetSocketLocation(AttachEndToSocket)
			: (bIsLocal ? EndComponent->GetComponentTransform().TransformPosition(EndPoint) : EndComponent->GetComponentLocation());
	}

	return FVector::ZeroVector; 
}

void UChainComponent::CalculateChainPoint(bool bIsAttached, FComponentReference& AttachRef, FName AttachSocket, int32 PointIndex, bool bUseEndPoint )
{
	if (bIsAttached)
	{
		ChainPoints[PointIndex].bFree = false;

		if (AttachRef.OtherActor == nullptr)
		{
			ChainPoints[PointIndex].Position = bUseEndPoint ? (bIsLocal ? GetComponentLocation() + EndPoint : EndPoint) : GetComponentLocation();
		}
		else
		{
			USceneComponent* component = Cast<USceneComponent>(AttachRef.GetComponent(GetOwner()));
			if (component)
			{
				ChainPoints[PointIndex].Position = (AttachSocket != NAME_None)
					? component->GetSocketLocation(AttachSocket)
					: (bUseEndPoint && bIsLocal ? component->GetComponentTransform().TransformPosition(EndPoint) : component->GetComponentLocation());
			}
		}
	}
	else
	{
		ChainPoints[PointIndex].bFree = true;
	}
}