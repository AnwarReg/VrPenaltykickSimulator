

#include "UDPReceiver.h"
// You may or may not need this include now, but it doesn't hurt.
// If your BallActor Blueprint is needed for other reasons, keep it.
#include "BallActor.h" 
#include "Engine/Engine.h"
#include "Sockets.h"
#include "SocketSubsystem.h" 
#include "Interfaces/IPv4/IPv4Address.h" 

AUDPReceiver::AUDPReceiver()
{
    PrimaryActorTick.bCanEverTick = true;
    ListenSocket = nullptr;
}

AUDPReceiver::~AUDPReceiver()
{
    if (ListenSocket)
    {
        ListenSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
    }
}

void AUDPReceiver::BeginPlay()
{
    Super::BeginPlay();

    ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
    if (SocketSubsystem)
    {
        TSharedRef<FInternetAddr> ListenEndpointAddr = SocketSubsystem->CreateInternetAddr();
        ListenEndpointAddr->SetAnyAddress(); // Listen on all local network interfaces (0.0.0.0)
        ListenEndpointAddr->SetPort(8888);   // The port to listen on

        ListenSocket = SocketSubsystem->CreateSocket(NAME_DGram, TEXT("MyUDPListenerSocket"), true);
        if (ListenSocket && ListenSocket->Bind(*ListenEndpointAddr))
        {
            ListenSocket->SetNonBlocking(true);
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("UDP Listener started on port %d"), ListenEndpointAddr->GetPort()));
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("UDP Listener FAILED to bind"));
            if (ListenSocket)
            {
                SocketSubsystem->DestroySocket(ListenSocket);
                ListenSocket = nullptr;
            }
        }
    }
}

void AUDPReceiver::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    if (ListenSocket)
    {
        ListenSocket->Close();
        ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(ListenSocket);
        ListenSocket = nullptr;
    }
}

void AUDPReceiver::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    ListenForData();
}

void AUDPReceiver::ListenForData()
{
    if (!ListenSocket) return;

    TArray<uint8> DataBuffer;
    DataBuffer.SetNumUninitialized(1024);
    int32 BytesRead;

    if (ListenSocket->Recv(DataBuffer.GetData(), DataBuffer.Num(), BytesRead))
    {
        if (BytesRead > 0)
        {
            DataBuffer[BytesRead] = 0;
            FString ReceivedData = FString(ANSI_TO_TCHAR(reinterpret_cast<const char*>(DataBuffer.GetData())));
            ParseReceivedData(ReceivedData);
        }
    }
}

// THIS IS THE MODIFIED FUNCTION
void AUDPReceiver::ParseReceivedData(const FString& Data)
{
    TArray<FString> ParsedValues;
    Data.ParseIntoArray(ParsedValues, TEXT(","), true);

    if (ParsedValues.Num() >= 3)
    {
        // Get the acceleration values from the parsed data
        float AccX = FCString::Atof(*ParsedValues[0]);
        float AccY = FCString::Atof(*ParsedValues[1]);
        float AccZ = FCString::Atof(*ParsedValues[2]);

        // Create a Vector from the parsed values
        FVector ReceivedAccelerationVector(AccX, AccY, AccZ);

        // Display the values on the screen (for debugging)
        FString DebugText = FString::Printf(TEXT("Parsed Accel - X: %.2f, Y: %.2f, Z: %.2f"), AccX, AccY, AccZ);
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, DebugText);

        // --- Call the Blueprint event ---
        // This sends the data over to the Blueprint Event Graph for you to use.
        OnUdpDataReceived(ReceivedAccelerationVector);
    }
}
