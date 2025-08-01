#include "BallActor.h"

// Sets default values
ABallActor::ABallActor()
{
    // Set this actor to call Tick() every frame
    PrimaryActorTick.bCanEverTick = true;

    // Create and attach the ball mesh component
    BallMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BallMesh"));
    RootComponent = BallMesh;

    // Enable physics on the ball mesh
    BallMesh->SetSimulatePhysics(true);
    BallMesh->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void ABallActor::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ABallActor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

// Function to apply the received acceleration as force
void ABallActor::ApplyAcceleration(const FVector& Acceleration)
{
    // Apply the force (F = m * a), where m is the mass of the ball
    FVector ImpulseVector = Acceleration * BallMesh->GetMass();  // Get mass from mesh component

    // Apply the force to the ball mesh component to simulate acceleration
    BallMesh->AddImpulse(ImpulseVector, NAME_None, true);

}
