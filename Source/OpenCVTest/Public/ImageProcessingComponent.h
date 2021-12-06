// Nick Chalvatzakis 2021

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"

#include "ImageProcessingComponent.generated.h"


//UENUM()
//enum CompareHistogramAlgorithms {
//	CMP_CORREL = 0,
//	CMP_CHISQR = 1,
//	CMP_INTERSECT = 2,
//	CMP_HELLINGER = 3,
//	CMP_CHISQRALT = 4,
//	CMP_KL_DIV = 5,
//	CMP_EUCLIDIAN_DISTANCE = 6,
//	CMP_MATCH_SHAPES = 7
//};



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

public:	


	UFUNCTION(BlueprintCallable)
	float GetClosestResemblingImage(int& OutImageIndex);

	float GetMinFloatInArray(TArray<float>&,int size,int& IndexMin);
	
};

