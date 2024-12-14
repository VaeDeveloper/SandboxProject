// This is Sandbox Project.

#include "LDPathLengthActor.h"
#include "Components/SplineMeshComponent.h"

DEFINE_LOG_CATEGORY_STATIC(SplineActorLevelDesign, All, All)

/**
 * Default constructor. Sets default values for the actor's properties and initializes components.
 */
ALDPathLengthActor::ALDPathLengthActor(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

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
	if (! SplineMeshForComponent) return;
	int32 NumberOfSplinePoints = SplineComp->GetNumberOfSplinePoints();		

	// Create spline mesh components between spline points.
	for (int32 i = 0; i < NumberOfSplinePoints - 1; i++)
	{
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());

		SplineMeshComponent->SetStaticMesh(SplineMeshForComponent);
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->AttachToComponent(SplineComp, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetForwardAxis(ESplineMeshAxis::Z);

		SetInstanceMaterialAndParams(SplineMeshComponent);

		FVector StartPoint, StartTangent, EndPoint, EndTangent;
		SplineComp->GetLocationAndTangentAtSplinePoint(i, StartPoint, StartTangent, ESplineCoordinateSpace::Local);
		SplineComp->GetLocationAndTangentAtSplinePoint(i + 1, EndPoint, EndTangent, ESplineCoordinateSpace::Local);

		SplineMeshComponent->SetStartAndEnd(StartPoint, StartTangent, EndPoint, EndTangent);
		SplineMeshComponent->SetStartScale(FVector2D(SplineMeshScale, SplineMeshScale));
		SplineMeshComponent->SetEndScale(FVector2D(SplineMeshScale, SplineMeshScale));
	}
}

void ALDPathLengthActor::SetInstanceMaterialAndParams(USplineMeshComponent* SplineMesh) 
{
	if (! MaterialInstance) return;

	UMaterialInstanceDynamic* DynamicMaterial = SplineMesh->CreateDynamicMaterialInstance(0, MaterialInstance);
	if (! DynamicMaterial) return;

	DynamicMaterial->SetVectorParameterValue(MaterialParameterName, MaterialColor);
}
