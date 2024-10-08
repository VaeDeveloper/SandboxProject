// This is Sandbox Project. 


#include "SplineChainComponent.h"
#include "Components/SplineComponent.h"

/**
 * Constructor for USplineChainComponent.
 * Initializes the spline follow weight curve and sets default attachment values.
 *
 * @param ObjectInitializer Used to initialize the object.
 */
USplineChainComponent::USplineChainComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SplineFollowWeight.EditorCurveData.AddKey(0, 0);
	SplineFollowWeight.EditorCurveData.AddKey(0.05, 1);
	SplineFollowWeight.EditorCurveData.AddKey(0.95, 1);
	SplineFollowWeight.EditorCurveData.AddKey(1, 0);
	AttachStart = false;
	AttachEnd = false;
}

/**
 * Called when the component is registered with its owning actor.
 * It tries to find the attached USplineComponent and initialize the chain based on it.
 *
 * If the spline component is found, it calculates the endpoint and initializes the chain accordingly.
 */
void USplineChainComponent::OnRegister()
{
	Super::OnRegister();

	if (!SplineComponent)
	{
		TArray<USceneComponent*> ChildrenComponents;
		GetChildrenComponents(false, ChildrenComponents);
		
		for (auto& ChildrenComponent : ChildrenComponents)
		{
			if (ChildrenComponent->IsA(USplineComponent::StaticClass()))
			{
				SplineComponent = Cast<USplineComponent>(ChildrenComponent);
				if (SplineComponent)
				{
					if (bIsLocal)
					{
						EndPoint = SplineComponent->GetLocationAtTime(1, ESplineCoordinateSpace::Local);
					}
					else
					{
						EndPoint = SplineComponent->GetLocationAtTime(1, ESplineCoordinateSpace::Local);
					}
					
					this->InitChain();
					return;
				}
			}
		}

		const auto Owner = GetOwner();

		if (Owner)
		{
			const auto ChildComponent = Owner->GetComponentByClass(USplineComponent::StaticClass());
			if (ChildComponent)
			{
				SplineComponent = Cast<USplineComponent>(ChildComponent);

				if (SplineComponent)
				{
					if (bIsLocal)
					{
						EndPoint = SplineComponent->GetLocationAtTime(1, ESplineCoordinateSpace::Local);
					}
					else
					{
						EndPoint = SplineComponent->GetLocationAtTime(1, ESplineCoordinateSpace::World);
					}
					this->InitChain();
				}
			}
		}
	}
	else
	{
		if (bIsLocal)
		{
			EndPoint = SplineComponent->GetLocationAtTime(1, ESplineCoordinateSpace::Local);
		}
		else
		{
			EndPoint = SplineComponent->GetLocationAtTime(1, ESplineCoordinateSpace::Local);
		}
		this->InitChain();
	}
}

/**
 * Called every frame to update the component's behavior.
 * This function checks if the spline follow weight has changed and updates the chain points accordingly.
 *
 * @param DeltaTime Time elapsed since last frame.
 * @param TickType Type of tick for this component.
 * @param ThisTickFunction Contains tick information for this component.
 */
void USplineChainComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (SplineComponent)
	{
#if WITH_EDITOR

		float Check = 0;

		for (auto& Key : SplineFollowWeight.GetRichCurveConst()->GetConstRefOfKeys())
		{
			Check += Key.Value;
		}

		if (Check != SplineFollowWeightCheck)
		{
			SplineFollowWeightCheck = Check;
			InitChain();
		}
#endif
		for (auto& Point : ChainPoints)
		{
			bool FollowWiightMore = SplineFollowWeight.GetRichCurveConst()->Eval(Point.Time) < 0.5f;

			FollowWiightMore ? (Point.bFree = false) : (Point.bFree = true);
			
			//if (SplineFollowWeight.GetRichCurveConst()->Eval(point.Time) < 0.5f)
			//{
			//	point.bFree = false;
			//}
			//else
			//{
			//	point.bFree = true;
			//}
		}
	}
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/**
 * Initializes the chain by setting the start and end points and creating the chain instances.
 * It uses the spline component to determine the position of each chain segment along the spline.
 */
void USplineChainComponent::InitChain()
{
	if (SplineComponent)
	{
		InstanceComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		ChainPoints.Empty();
		ChainPoints.AddUninitialized(Segments);
		ChainStart = GetComponentLocation();
		if (bIsLocal)
		{
			ChainEnd = GetComponentLocation() + EndPoint;
		}
		else
		{
			ChainEnd = EndPoint;
		}
		// const FVector LengthVector = ChainEnd - ChainStart;
		InstanceComponent->SetStaticMesh(ChainMesh);
		InstanceComponent->ClearInstances();

		if (InstanceComponent->GetInstanceCount() == 0)
		{
			float SegmentTime = 1.0f / static_cast<float>(Segments);
			SegmentLength = (SplineComponent->GetSplineLength() / static_cast<float>(Segments)) * ChainLength;

			for (int i = 0; i < Segments; i++)
			{
				FChainPointData Point = FChainPointData();
				Point.Time = static_cast<float>(i) * SegmentTime;
				Point.Position = SplineComponent->GetLocationAtDistanceAlongSpline(i * SegmentLength, ESplineCoordinateSpace::World);
				Point.OldPosition = Point.Position;
				Point.Transform.SetScale3D(Scale);
				ChainPoints[i] = Point;
				InstanceComponent->AddInstance(Point.Transform);
			}
		}
	}
	else
	{
		Super::InitChain();
	}
}
