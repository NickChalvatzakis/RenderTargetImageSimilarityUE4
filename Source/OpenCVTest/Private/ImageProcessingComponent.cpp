// Nick Chalvatzakis 2021


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


/* Get Data from an array of saved Texture2D Images and the currently drawn RenderTarget and compare them */
float UImageProcessingComponent::GetClosestResemblingImage(int& OutImageIndex) {

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
		cv::Mat wrappedRenderTarget(RenderTargetResource->GetSizeX(), RenderTargetResource->GetSizeY(), CV_8UC4, buffer.GetData());
		cv::imwrite("D:/P4/ImageCheck/rt.png", wrappedRenderTarget); //for Debugging reasons - can be ommited

		//Gray scale render target
		Mat grayRender;
		cv::cvtColor(wrappedRenderTarget, grayRender, COLOR_RGBA2GRAY);
		cv::imwrite("D:/P4/ImageCheck/rtGray.png", grayRender); //for Debugging reasons - can be ommited


		//array of comparisons for each image
		TArray<float> comps;
		comps.Init(0, n);


		for (int i = 0; i < n; i++) {
			comps[i] = cv::matchShapes(grayRender, *wrappedImages[i], CONTOURS_MATCH_I1, 0);
		}


		comparisonRate = GetMinFloatInArray(comps, n, OutImageIndex);
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
