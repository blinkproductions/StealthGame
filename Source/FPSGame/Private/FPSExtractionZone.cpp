// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Overlap Component"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.f));
	SetRootComponent(OverlapComp);

	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal Component"));
	DecalComp->DecalSize = FVector(200.f);
	DecalComp->AttachTo(RootComponent);
}


void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr) //if MyPawns is not null AND MyPawnIsCarryingObjective
	{
		return;
	}
	
	if (MyPawn->bIsCarryingObjective)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlap with extraction zone!"));
		
		AFPSGameMode* GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->CompleteMission(MyPawn, true);
		}
	}
	else {
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}

}
