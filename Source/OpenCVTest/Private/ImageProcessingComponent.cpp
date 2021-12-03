// Fill out your copyright notice in the Description page of Project Settings.


#include "ImageProcessingComponent.h"
#include "Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Engine/Classes/Kismet/KismetRenderingLibrary.h"
#include "Engine/Texture2DArray.h"

// Sets default values for this component's properties
UImageProcessingComponent::UImageProcessingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UImageProcessingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UImageProcessingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// ...
}
/* Get Data from an array of saved Texture2D Images and the currently drawn RenderTarget and compare them */
float UImageProcessingComponent::GetClosestResemblingImage(TEnumAsByte<CompareHistogramAlgorithms> compareCode, int& OutImageIndex) {

	//Return this
	double comparisonRate = 0;



	const int n = ImageLibrary.Num();
	TArray<Mat*> wrappedImages;

	//TODO : Get the data from an array of images
	for (UTexture2D* texture : ImageLibrary) {
		FColor* FormatedImageData = static_cast<FColor*>(texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_ONLY));

		wrappedImages.Add(new Mat(texture->PlatformData->SizeX, texture->PlatformData->SizeY, CV_8UC1, FormatedImageData));
		imwrite("D:/P4/ImageCheck/img" + std::to_string(wrappedImages.Find(wrappedImages.Last())) + ".png", *wrappedImages.Last()); //for Debugging reasons - can be ommited
		texture->PlatformData->Mips[0].BulkData.Unlock();
	}

	//Get Data from render target
	FTextureRenderTargetResource* RenderTargetResource = RenderTarget->GameThread_GetRenderTargetResource();

	TArray<FColor> buffer;

	if (RenderTargetResource) {

		//Read pixels and save them to a color buffer, then pass that buffer to a new Mat object with the correct dimensions.
		RenderTargetResource->ReadPixels(buffer);
		cv::Mat wrappedRenderTarget(RenderTargetResource->GetSizeX(), RenderTargetResource->GetSizeY(),CV_8UC4,buffer.GetData() );
		cv::imwrite("D:/P4/ImageCheck/rt.png", wrappedRenderTarget);
		
		//Gray scale render target
		Mat grayRender;
		cv::cvtColor(wrappedRenderTarget, grayRender, COLOR_RGBA2GRAY);
		cv::imwrite("D:/P4/ImageCheck/rtGray.png", grayRender); //for Debugging reasons - can be ommited
		

		//HSV rendertarget
		cv::cvtColor(wrappedRenderTarget, wrappedRenderTarget, COLOR_RGBA2BGR);
		cv::cvtColor(wrappedRenderTarget, wrappedRenderTarget, COLOR_BGR2HSV);
		cv::imwrite("D:/P4/ImageCheck/rtHSV.png", wrappedRenderTarget); //for Debugging reasons - can be ommited

		//Calculate Histograms parameters
		int bins = 16;
		int histiSize = 256;
		float range[] = { 0, 256 };
		const float* histiRange[] = { range };

		//GrayScale histogram
		Mat grayHistRend;
		calcHist(&grayRender, 1, 0, Mat(), grayHistRend, 1, &histiSize, histiRange, true, false);
		float norm1 = norm(grayRender, NORM_L2, Mat());

		//array of comparisons for each image
		TArray<float> comps;
		comps.Init(0, n);
		
		if (compareCode == CMP_EUCLIDIAN_DISTANCE) {

			//Matrix that contains the diffrence of points between pixels
			cv::Mat difMat(grayRender.rows, grayRender.cols, CV_8UC1);

			
			for (int i = 0; i < n; i++) {
				/*for (int x = 0; x < grayRender.rows; x++) {
					for (int y = 0; y < grayRender.cols; y++) {
						
					}
				}*/
				comps[i] = norm(grayRender, *wrappedImages[i], NORM_L2, noArray());
				comps[i] = (grayRender.rows * grayRender.cols) / comps[i];
				//open cv norm
				//comps[i] = norm(difMat, NORM_L2, noArray());
			}
			comparisonRate = GetMinFloatInArray(comps,n, OutImageIndex);
		}
		else {

			Mat tempHist;
			for (int i = 0; i < n; i++) {
				calcHist(wrappedImages[i], 1, 0, Mat(), tempHist, 1, &histiSize, histiRange, true, false);
				comps[i] = compareHist(grayHistRend, tempHist, compareCode.GetValue());
			}
			comparisonRate = GetMinFloatInArray(comps, n, OutImageIndex);
		}
		UE_LOG(LogTemp, Log, TEXT("%f this is the rate with %d method"), comparisonRate, compareCode.GetValue());
	}

	UE_LOG(LogTemp, Log, TEXT("Final Output In Function : %d . %f"), OutImageIndex, comparisonRate);

	return comparisonRate;
}

/*Get the minimum float in an array and the according index of the image*/
float UImageProcessingComponent::GetMinFloatInArray(TArray<float>& c,int size,int& IndexMin) {
	float min = INT_MAX;
	for (int i = 0; i < size; i++) {
		if (c[i] < min) {
			min = c[i];
			IndexMin = i;
		}
		UE_LOG(LogTemp, Log, TEXT("%d . %f"), i, c[i]);
	}

	return min;
}