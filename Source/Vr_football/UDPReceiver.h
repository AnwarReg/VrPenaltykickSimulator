#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Networking.h"
#include "Sockets.h"
#include "UDPReceiver.generated.h"

UCLASS()
class VR_FOOTBALL_API AUDPReceiver : public AActor
{
    GENERATED_BODY()

public:
    AUDPReceiver();
    ~AUDPReceiver(); // Destructor for cleanup

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

    // This is a C++ function that creates an event node in the Blueprint editor.
    // We will call this from our C++ code to send the received data to our Blueprint Graph.
    UFUNCTION(BlueprintImplementableEvent, Category = "UDP")
    void OnUdpDataReceived(const FVector& ReceivedAcceleration);

public:
    virtual void Tick(float DeltaTime) override;

private:
    FSocket* ListenSocket;
    void ListenForData();
    void ParseReceivedData(const FString& Data);
};