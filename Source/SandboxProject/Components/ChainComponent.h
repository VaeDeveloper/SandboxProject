// This is Sandbox Project. 

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/Engine.h"
#include "UObject/ObjectMacros.h"

#include "ChainComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSoundReached, const FVector, Velocity);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChainCollide, const TArray<FHitResult>, HitResult);

class UInstancedStaticMeshComponent;
class UStaticMesh;

/**
 * Struct containing information about a point along the cable.
 * This structure represents a point in a chain simulation, holding data
 * related to its position, velocity, forces acting upon it, and other
 * relevant properties for dynamic simulation.
 */
USTRUCT(BlueprintType)
struct FChainPointData
{
	GENERATED_BODY()

	FChainPointData()
		: bFree(true)
		, Position(0, 0, 0)
		, OldPosition(0, 0, 0)
		, Velocity(0, 0, 0)
		, Force(0, 0, 0)
		, Rotation(0, 0, 0)
		, Transform(Rotation, Position)
		, Time(0)
		, Index(0)
	{}

	/**
	 * Indicates whether this point is free (simulating) or fixed to another object.
	 * A value of true means the point is free to move based on simulation physics.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	bool bFree;

	/**
	 * The current position of the chain point in 3D space.
	 * This vector represents the x, y, z coordinates of the point.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FVector Position;

	/**
	 * The position of the chain point in the previous iteration of the simulation.
	 * Used to calculate movement and velocity.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FVector OldPosition;

	/**
	 * The current velocity of the chain point.
	 * This vector indicates the speed and direction of movement at this point.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FVector Velocity;

	/**
	 * The force acting on the chain point.
	 * This vector represents the sum of forces (e.g., tension, gravity) applied to the point.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FVector Force;

	/**
	 * The rotation of the chain point.
	 * This struct contains pitch, yaw, and roll values that define the orientation.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FRotator Rotation;

	/**
	 * The transformation matrix of the chain point.
	 * This combines the rotation and position into a single transform.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FTransform Transform;

	/**
	 * The time value associated with the chain point, used for spline interpolation.
	 * This value can help determine the point's position along a spline curve.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	float Time;

	/**
	 * The index of the chain point within the overall chain structure.
	 * This helps in identifying the order or position of the point in the chain.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	int Index;

	/**
	 * The direction of the chain point.
	 * This vector indicates the direction the point is facing or moving towards.
	 */
	UPROPERTY(BlueprintReadOnly, Category = "ChainComponent")
	FVector Direction;
};


/**
 * UChainComponent is a custom Unreal Engine component that handles chain-like behavior.
 * It extends the UMeshComponent and provides functionality for rendering, physics, and sound events
 * related to chain interactions within a game.
 *
 * This component allows for configuration of chain properties such as length, segments,
 * collision behavior, and attachment points, facilitating the creation of dynamic chain systems
 * in a game environment.
 */
UCLASS(HideCategories = (Object, Physics, Activation, "Components|Activation"), EditInlineNew, meta = (BlueprintSpawnableComponent), ClassGroup = Rendering, Blueprintable)
class SANDBOXPROJECT_API UChainComponent : public UMeshComponent
{
	GENERATED_BODY()

public:
	UChainComponent(const FObjectInitializer& ObjectInitializer);
	virtual void OnRegister() override;
	virtual void OnUnregister() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void BeginPlay() override;

	/**
	 * Applies a force to the chain at a specified position within a given radius.
	 *
	 * @param InPosition The world position where the force will be applied.
	 * @param InRadius The radius within which the force affects the chain segments.
	 * @param InForce The vector representing the force to be applied.
	 * @return An array of integers representing the indices of the affected chain points.
	 */
	UFUNCTION(BlueprintCallable, Category = "ChainComponent|Chain Component")
	TArray<int> ApplyForce(FVector InPosition, float InRadius, FVector InForce);

	/**
	 * Retrieves the position of a specific point in the chain.
	 *
	 * @param index The index of the chain point to retrieve.
	 * @return The FVector representing the position of the specified chain point.
	 */
	UFUNCTION(BlueprintCallable, Category = "ChainComponent|Chain Component")
	FVector GetChainPoint(int index);

	/**
	 * Attaches the start of the chain to a specified actor at a designated socket.
	 *
	 * @param ComponentReference A reference to the component of the actor to attach to.
	 * @param Socket The name of the socket to attach the start of the chain to.
	 */
	UFUNCTION(BlueprintCallable, Category = "ChainComponent|Chain Component")
	void AttachStartToActor(FComponentReference ComponentReference, FName Socket);

	/**
	 * Attaches the end of the chain to a specified actor at a designated socket.
	 *
	 * @param ComponentReference A reference to the component of the actor to attach to.
	 * @param Socket The name of the socket to attach the end of the chain to.
	 */
	UFUNCTION(BlueprintCallable, Category = "ChainComponent|Chain Component")
	void AttachEndToActor(FComponentReference ComponentReference, FName Socket);

	/**
	 * Gets the array of points that define the chain.
	 *
	 * @return An array of FChainPointData containing information about each chain point.
	 */
	UFUNCTION(BlueprintCallable, Category = "ChainComponent|Chain Component")
	FORCEINLINE TArray<FChainPointData> GetChainPoints()
	{
		return ChainPoints;
	}

protected:

	/**
	 * Initializes the chain's parameters and properties.
	 * This function is called at the start to set up the chain.
	 */
	virtual void InitChain();

	/**
	 * Draws debug visuals for the chain points in the editor.
	 * This helps in visualizing the state and position of the chain during development.
	 */
	void DrawChainPoints();

	/**
	 * Updates the attachment points for the start and end of the chain.
	 * This ensures that the chain is correctly anchored to the specified components.
	 */
	void UpdateAttachments();

	/**
	 * Calculates the positions of the points in the chain based on physics constraints.
	 * This method determines the spatial configuration of the chain segments.
	 */
	void CalculatePoints();

	/**
	 * Applies gravity to the chain segments to simulate realistic falling behavior.
	 * This method updates the positions of the chain points based on gravitational forces.
	 */
	void ApplyGravity();

	/**
	 * Solves the distance constraints between the points in the chain.
	 * This ensures that the segments maintain their defined lengths.
	 */
	void SolveConstraint();

	/**
	 * Resolves any collisions that occur between chain segments or with other objects.
	 * This method checks for overlapping points and adjusts their positions accordingly.
	 */
	void ResolveCollision();

	/**
	 * Updates the instanced mesh component to reflect the current state of the chain.
	 * This method refreshes the visual representation of the chain based on the calculated points.
	 */
	void UpdateMeshes();

	/**
	 * Updates the position of a specific point in the chain based on its neighbors and length constraints.
	 *
	 * @param i The index of the point to update.
	 * @param p1 The first neighboring point.
	 * @param p2 The second neighboring point.
	 * @param length The desired length between the points.
	 * @param offset An integer offset for additional adjustments.
	 */
	void UpdatePoint(size_t i, FChainPointData& p1, FChainPointData& p2, float length, int offset);

	/**
	 * The starting point of the chain in the world space.
	 */
	FVector ChainStart, ChainEnd;

	/**
	 * A frame counter used to skip certain calculations for performance optimization.
	 */
	int Frame = 0;

	/**
	 * The length of each segment in the chain.
	 */
	float SegmentLength;


	/**
	 * An array holding the data for each point in the chain.
	 */
	TArray<FChainPointData> ChainPoints;


	//---data---
public:
	/**
	 * The interpolation factor used for chain movement.
	 */
	UPROPERTY(EditAnywhere, Category = "ChainComponent|Chain Component")
	float Lerp = 0.1;

	/**
	 * Delegate called when the chain reaches a sound threshold.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ChainComponent|Chain Component")
	FOnSoundReached OnSoundReached;

	/**
	 * Delegate called when the chain collides with an object.
	 */
	UPROPERTY(BlueprintAssignable, Category = "ChainComponent|Chain Component")
	FOnChainCollide OnCollide;

	/**
	 * Reference to the static mesh used for the chain.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|Chain", meta = (ShortToolTip = "Chains static mesh reference"))
	UStaticMesh* ChainMesh;

	/**
	 * Scale transform applied to each chain segment.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|Chain", meta = (ShortToolTip = "Transform scale of each chain"))
	FVector Scale = FVector::OneVector;

	/**
	 * Number of segments in the chain.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|Chain", meta = (ShortToolTip = "Number of chain segments"))
	int Segments = 10;

	/**
	 * Incremental rotation applied to each chain segment.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|Chain", meta = (ShortToolTip = "Incremental Transform rotation of each chain"))
	FVector AdditiveRotation = FVector(0, 0, 45);

	/**
	 * The rest length of the chain segments.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|Chain", meta = (ShortToolTip = "Rest length of chains"))
	float ChainLength = 1.5f;

	/**
	 * Determines if the chain uses local space for its transformations.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", meta = (ShortToolTip = "Is Local space", MakeEditWidget = true))
	bool bIsLocal = true;

	/**
	 * The endpoint location of the chain.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", meta = (ShortToolTip = "Location of the end point", MakeEditWidget = true))
	FVector EndPoint = FVector(100, 0, 0);

	/**
	 * Determines if the chain is attached at the start point.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", meta = (ShortToolTip = "Is attached to start point"))
	bool AttachStart = true;

	/**
	 * Reference to the component to attach the start point to.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", META = (ShortToolTip = "Start point attachment component reference"))
	FComponentReference AttachStartTo;

	/**
	 * The socket name for the start point attachment.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", META = (ShortToolTip = "Start point attachment socket name"))
	FName AttachStartToSocket;

	/**
	 * Determines if the chain is attached at the end point.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", META = (ShortToolTip = "Is attached to end point"))
	bool AttachEnd = true;

	/**
	 * Reference to the component to attach the end point to.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", META = (ShortToolTip = "End point attachment component reference"))
	FComponentReference AttachEndTo;

	/**
	 * The socket name for the end point attachment.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttach", META = (ShortToolTip = "End point attachment socket name"))
	FName AttachEndToSocket;

	/**
	 * Reference to the component for attaching to the start point.
	 */
	UPROPERTY(EditAnyWhere, Category = "ChainComponent|ChainAttachment", meta = (ShortToolTip = "Start point attachment component reference"))
	FComponentReference AttachComponentToStart;

	/**
	 * Determines if the start attachment component should be rotated.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttachment", meta = (ShortToolTip = "Rotate start attachment component"))
	bool RotateStartAttachment = true;

	/**
	 * Reference to the component for attaching to the end point.
	 */
	UPROPERTY(EditAnyWhere, Category = "ChainComponent|ChainAttachment", meta = (ShortToolTip = "Start point attachment component reference"))
	FComponentReference AttachComponentToEnd;

	/**
	 * Determines if the end attachment component should be rotated.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainAttachment", meta = (ShortToolTip = "Rotate start attachment component"))
	bool bRotateEndAttachment = true;

	/**
	 * The gravity scale applied to the chains.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainPhysic", meta = (ShortToolTip = "Gravity scale of chains"))
	float Gravity = 0.98f;

	/**
	 * The stiffness of the chains.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainPhysic", meta = (UIMin = 1.0, ShortToolTip = "Stiffness of chains"))
	int Stiffness = 10;

	/**
	 * The friction coefficient of the chains.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainPhysic", meta = (UIMin = 0.1, UIMax = 1.0, ShortToolTip = "Friction of chains"))
	float Friction = 0.3f;

	/**
	 * The width of the chain for world collision detection.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainCollision", meta = (ShortToolTip = "Chains width for world collision detection"))
	float ChainWidth = 20;

	/**
	 * Determines if self-collision is enabled for the chains.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainCollision", meta = (UIMin = 0.0, UIMax = 1.0, ShortToolTip = "Is self collision enabled"))
	bool bSelfCollision = false;

	/**
	 * The width for self-collision detection of the chains.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainCollision", meta = (ShortToolTip = "Chains width for self collision detection"))
	float SelfCollisionWidth = 20;

	/**
	 * The threshold for self-collision detection of the chain.
	 * If the force exceeds this value, self-collision will be triggered.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "ChainComponent|ChainCollision", meta = (ShortToolTip = "Self collision force threshold"))
	float SelfCollisionThreshold = 0.05f;

	/**
	 * The number of frames to skip when rendering the chain.
	 * This allows for optimization by reducing the update frequency.
	 * Default value is 0.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|ChainRender", meta = (UIMin = 0.0, ShortToolTip = "Chains skip counter by frame"))
	int Skip = 0;

	/**
	 * Determines if debug visualization for the chains is enabled.
	 * If true, debug drawings will be displayed in the editor.
	 * Default value is false.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|ChainRender", meta = (ShortToolTip = "Is chains debug drawer enabled"))
	bool bDrawDebugger = false;

	/**
	 * Reference to the instanced static mesh component used for the chain.
	 * This component handles the instancing of the chain meshes for better performance.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|ChainRender", meta = (UIMin = 0.0, ShortToolTip = "Instanced component object reference"))
	UInstancedStaticMeshComponent* InstanceComponent;

	/**
	 * The velocity threshold for triggering the OnSoundReached event.
	 * When the chain's velocity exceeds this value, the event will be called.
	 * Default value is 1.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|ChainSound", meta = (UIMin = 0.0, ShortToolTip = "Chains velocity threshold for OnSoundReach event call"))
	float SoundThreshold = 1;

	/**
	 * The number of frames to skip before calling the OnSoundReached event.
	 * This helps in controlling the frequency of the event calls.
	 * Default value is 1.
	 */
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "ChainComponent|ChainSound", meta = (UIMin = 0.0, ShortToolTip = "Skip counter by frame for calling OnSoundReach event call"))
	int SoundSkip = 1;


private:
	void CalculateChainEnd(FVector& ChainEndResult);
	void CalculateChainPoint(bool bIsAttached, FComponentReference& AttachRef, FName AttachSocket, int32 PointIndex, bool bUseEndPoint = false);
	FVector GetChainEndPoint() const;



};
