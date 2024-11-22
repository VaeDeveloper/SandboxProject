// This is Sandbox Project. 


#include "LDPathLengthActor.h"

// Sets default values
ALDPathLengthActor::ALDPathLengthActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	check(SplineComp);

	TextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextRender"));
	check(TextRender);



}



