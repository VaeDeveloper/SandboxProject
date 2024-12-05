// This is Sandbox Project.

#include "LDPathLengthActor.h"
#include "Components/SplineMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SplineActorLevelDesign, All, All)

/**
 * Default constructor. Sets default values for the actor's properties and initializes components.
 */
ALDPathLengthActor::ALDPathLengthActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SetRootComponent(SplineComp);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	TextRender->SetRelativeRotation(FRotator(0.0, 90.0, 0.0));
	TextRender->SetupAttachment(SplineComp);
}

/**
 * Called in the editor when the actor is constructed. Updates text and spline meshes.
 */
void ALDPathLengthActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SetTextParams();
	ConstructSplineMeshComponent(Transform);
}

bool ALDPathLengthActor::Modify(bool bAlwaysMarkDirty)
{
	UE_LOG(SplineActorLevelDesign, Error, TEXT("=======Modify======="));
	return Super::Modify(bAlwaysMarkDirty);
}

//void ALDPathLengthActor::PostEditUndo() 
//{
//	Super::PostEditUndo();
//	UE_LOG(SplineActorLevelDesign, Warning, TEXT("=======PostEditUndo======= "));
//}

//bool ALDPathLengthActor::Modify(bool bAlwaysMarkDirty)
//{
//	return Super::Modify(bAlwaysMarkDirty);
//
//	UE_LOG(SplineActorLevelDesign, Warning, TEXT("=======Modify======= "));
//}




/**
 * Updates the text render component based on the spline's length and selected measurement unit.
 */
void ALDPathLengthActor::SetTextParams()
{
	SplineLength = SplineComp->GetSplineLength();
	TextRender->SetWorldSize(TextRenderSize);

	// Convert length to meters.
	float SplineLengthMeters = SplineLength / 100.0f;

	// Update the text based on the selected measurement unit.
	switch (WorldValue)
	{
		case ELengthWolrdValue::ELWV_Meters:
		{
			FString LengthStr = FString::Printf(TEXT("%.2f m"), SplineLengthMeters);
			TextRender->SetText(FText::FromString(LengthStr));
			break;
		}

		case ELengthWolrdValue::ELWV_Cent:
		{
			FString LengthStringMeters = FString::Printf(TEXT("%.2f cm"), SplineLength);
			TextRender->SetText(FText::FromString(LengthStringMeters));
			break;
		}

		default: break;
	}
}

/**
 * Constructs spline mesh components along the spline and applies dynamic materials.
 */
void ALDPathLengthActor::ConstructSplineMeshComponent(const FTransform& SplineTransform)
{
	(void) SplineTransform;

	for (auto SMeshComponent : SplineMeshComponents)
	{
		if (IsValid(SMeshComponent))
		{
			UE_LOG(SplineActorLevelDesign, Warning, TEXT("Removing component: %s"), *SMeshComponent->GetName());
			SMeshComponent->UnregisterComponent();
			SMeshComponent->DestroyComponent();
			SMeshComponent->ConditionalBeginDestroy();
			SMeshComponent->MarkAsGarbage();
			SMeshComponent->MarkRenderStateDirty();
			// SMeshComponent->ConditionalFinishDestroy();
		}
		else
		{
			UE_LOG(SplineActorLevelDesign, Error, TEXT("Invalid component encountered during cleanup."));
		}
	}

	SplineMeshComponents.Empty();

	GEngine->ForceGarbageCollection(true);


	int32 NumberOfSplinePoints = SplineComp->GetNumberOfSplinePoints();		

	UE_LOG(SplineActorLevelDesign, Warning, TEXT("Points:  %i"), NumberOfSplinePoints);
	UE_LOG(SplineActorLevelDesign, Warning, TEXT("Segment: %i"), SplineComp->GetNumberOfSplineSegments());

	int32 MeshCount = 0;

	// Create spline mesh components between spline points.
	for (int32 i = 0; i < NumberOfSplinePoints - 1; ++i)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this);

		if (! SplineMeshComponent)
		{
			UE_LOG(SplineActorLevelDesign, Error, TEXT("Failed to create SplineMeshComponent."));
			continue;
		}

		SplineMeshComponent->RegisterComponent();
		SplineMeshComponent->SetRelativeTransform(SplineComp->GetRelativeTransform());

		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);

		if (! SplineMeshForComponent) return;

		SplineMeshComponent->SetStaticMesh(SplineMeshForComponent);

		SetInstanceMaterialAndParams(SplineMeshComponent);

		FVector StartPoint, StartTangent, EndPoint, EndTangent;
		SplineComp->GetLocationAndTangentAtSplinePoint(i, StartPoint, StartTangent, ESplineCoordinateSpace::Local);
		SplineComp->GetLocationAndTangentAtSplinePoint(i + 1, EndPoint, EndTangent, ESplineCoordinateSpace::Local);

		// SplineMeshComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
		SplineMeshComponent->SetStartScale(FVector2D(SplineMeshScale, SplineMeshScale));
		SplineMeshComponent->SetEndScale(FVector2D(SplineMeshScale, SplineMeshScale));

		SplineMeshComponents.Add(SplineMeshComponent);
	}

	MeshCount = SplineMeshComponents.Num();
	UE_LOG(SplineActorLevelDesign, Error, TEXT("Spline Mesh Count:  %i"), MeshCount);
}

void ALDPathLengthActor::SetInstanceMaterialAndParams(USplineMeshComponent* SplineMesh) 
{
	if (! MaterialInstance) return;

	UMaterialInstanceDynamic* DynamicMaterial = SplineMesh->CreateDynamicMaterialInstance(0, MaterialInstance);
	if (! DynamicMaterial) return;

	DynamicMaterial->SetVectorParameterValue(MaterialParameterName, MaterialColor);
}
