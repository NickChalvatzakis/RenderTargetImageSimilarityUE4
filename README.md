# RenderTargetImageSimilarityUE4
OpenCV &amp; UE4. 

Take data from the render target and an array of images and check them for similarity of some degree.

I implemented a lot of tests like histogram checking, Euclidian Distance (Norm L2), cross correlation. But OpenCV features a handy little function called [cv::matchShapes](https://docs.opencv.org/4.x/d3/dc0/group__imgproc__shape.html#gaadc90cb16e2362c9bd6e7363e6e4c317)

Here is a video where I'm comparing two images i saved as static Texture2D images and the Render Target. 

[Video](https://www.youtube.com/watch?v=AOiE3Ms9T7E&feature=youtu.be)

I will continue my reseach, I hope to extend the UE4 editor to the imshow functionalities to so I can quickly conduct analysis rather than manually saving the images and checking them.



