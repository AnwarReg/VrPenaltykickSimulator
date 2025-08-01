#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BallActor.generated.h"

UCLASS()
class VR_FOOTBALL_API ABallActor : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ABallActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Static Mesh Component to represent the ball
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* BallMesh;

    // Function to apply the received acceleration as force
    UFUNCTION(BlueprintCallable, Category = "BallPhysics")
    void ApplyAcceleration(const FVector& Acceleration);

    // You can also add variables like KickPowerScale here if needed
};
