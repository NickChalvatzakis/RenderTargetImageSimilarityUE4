// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "ImageProcessingComponent.generated.h"


UENUM()
enum CompareHistogramAlgorithms {
	CMP_CORREL = 0,
	CMP_CHISQR = 1,
	CMP_INTERSECT = 2,
	CMP_HELLINGER = 3,
	CMP_CHISQRALT = 4,
	CMP_KL_DIV = 5,
	CMP_EUCLIDIAN_DISTANCE = 6
};



using namespace cv;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPENCVTEST_API UImageProcessingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UImageProcessingComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextureRenderTarget2D* RenderTarget; //

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTexture2D* StaticImageTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UTexture2D*> ImageLibrary;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//	TEnumAsByte<CompareHistogramAlgorithms> compareCode;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	float GetClosestResemblingImage(TEnumAsByte<CompareHistogramAlgorithms> compareCode,int& OutImageIndex);

	float GetMinFloatInArray(TArray<float>&,int size,int& IndexMin);

	float GetMaxFloatArray(TArray<float>& c, int size, int& IndexMax);
	
};

