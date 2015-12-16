#include "fisherfaceclassifer.h"
#include <opencv\highgui.h>
#include <opencv\cvaux.h>
#include <QMessageBox>
#include <limits>
#include <QDebug>
#include "logger.h"
#include "ImageUtils.h"
namespace cognition
{   const int BORDER = 8;  // Border between GUI elements to the edge of the image.
    vector<Mat> fisherfaceclassifer::csvimages;
    // C++ conversion functions between integers (or floats) to std::string.
    template <typename T> string toString(T t)
    {
    ostringstream out;
    out << t;
    return out.str();
   }
    Ptr<FaceRecognizer> fisherfaceclassifer :: model;
    static Mat norm_0_255(InputArray _src) {
    Mat src = _src.getMat();
    // Create and return normalized image:
    Mat dst;
    switch(src.channels()) {
    case 1:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC1);
        break;
    case 3:
        cv::normalize(_src, dst, 0, 255, NORM_MINMAX, CV_8UC3);
        break;
    default:
        src.copyTo(dst);
        break;
    }
    return dst;
}
    // Return the number of bits in each channel of the given Mat. ie: 8, 16, 32 or 64.
    int getBitDepth(const cv::Mat M)
   {
    switch (CV_MAT_DEPTH(M.type())) {
        case CV_8U:
        case CV_8S:
            return 8;
        case CV_16U:
        case CV_16S:
            return 16;
        case CV_32S:
        case CV_32F:
            return 32;
        case CV_64F:
            return 64;
       }
    return -1;
    }
    void printArray2D(const uchar *data, int cols, int rows, int channels, int depth_type, int step, int maxElements)
{
    char buff[32];
    if (data != 0 && cols > 0 && rows > 0 && channels > 0 && step > 0) {

        // Show the actual data values
        if (maxElements >= 0) {
            if (maxElements == 0)
                maxElements = rows * cols;
            int totalElements = 0;
            //int step = step;

            for (int row=0; row < rows; row++) {
                string s = "";
                int element = 0;
                if (channels > 1 && rows > 1) {
                    snprintf(buff, sizeof(buff), "row%d: ", row);
                }
                for (int col=0; col < cols; col++) {
                    if (channels > 1)
                        s += "[";
                    for (int ch=0; ch <= channels-1; ch++) {
                        if (ch > 0 || (channels == 1 && col != 0))    // Add a separator, except for the first element of this pixel or row.
                            s += ",";

                        buff[0] = '?';  // Initialize the string to "?" if something goes wrong.
                        buff[1] = 0;

                        // Allow to print just part of the image.
                        totalElements++;
                        if (totalElements > maxElements) {
                            // LOG can be printf or similar.
                             LOG("%s ... <just displaying the 1st %d entries from %d!>", s.c_str(), maxElements, rows * cols * channels);
                            return;
                        }

                        switch (depth_type) {
                        case CV_8U:
                        case CV_8S:         // 8-bit UCHAR Mat.
                            snprintf(buff, sizeof(buff), "%d", data[(row * step) + (col*channels) + ch]);
                            break;
                        case CV_16U:
                        case CV_16S:   // 16-bit short Mat.
                            snprintf(buff, sizeof(buff), "%d", *(short*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(short)]);
                            break;
                        case CV_32S:   // 32-bit int Mat.
                            snprintf(buff, sizeof(buff), "%d", *(int*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(int)]);
                            break;
                        case CV_32F:   // 32-bit float Mat.
                            snprintf(buff, sizeof(buff), "%.3f", *(float*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(float)]);
                            break;
                        case CV_64F:   // 64-bit double Mat.
                            snprintf(buff, sizeof(buff), "%.3lg", *(double*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(double)]);
                            break;
                        default:
                            snprintf(buff, sizeof(buff), "UNKNOWN DEPTH OF %d!", depth_type);
                        }
                        s += buff;

                        const int MAX_ELEMENTS_PER_LINE = 30;   // Only print upto 30 numbers per LOG() statement, since Android can only handle about 250 characters per log line!
                        if (element > MAX_ELEMENTS_PER_LINE) {
                            // LOG can be printf or similar.
                             Logger::getInstance().log(s.c_str());
                            s = "";
                            element = 0;
                        }
                        element++;
                    }
                    if (channels > 1)
                        s += "] ";
                }
                // LOG can be printf or similar.
                 Logger::getInstance().log(s.c_str());
            }
        }//end if (maxElements>=0)
    }
}
    // Print the label and then contents of a cv::Mat from the C++ interface (using "LOG()") for easy debugging.
    // If 'maxElements' is 0, it will print the whole array. If it is -1, it will not print the array at all.
    void printMat(const cv::Mat M, const char *label, int maxElements)
    {
    string s;
    char buff[32];
    if (label)
        s = label + string(": ");
    else
        s = "Mat: ";
    if (!M.empty()) {
        int channels = CV_MAT_CN(M.type());
        int depth_bpp = getBitDepth(M);     // eg: 8, 16, 32.
        int depth_type = CV_MAT_DEPTH(M.type());    // eg: CV_32S, CV_32F

        // Show the dimensions & data type
        sprintf(buff, "%dw%dh %dch %dbpp", M.cols, M.rows, channels, depth_bpp);
        s += string(buff);

        // Show the data range for each channel
        s += ", range";
        for (int ch=0; ch<channels; ch++) {
            cv::Mat arr = cv::Mat(M.rows, M.cols, depth_type);
            // Extract one channel at a time, to show it's range.
            int from_to[2];
            from_to[0] = ch;
            from_to[1] = 0;
            cv::mixChannels( &M, 1, &arr, 1, from_to, 1 );
            // Show it's range.
            double minVal, maxVal;
            cv::minMaxLoc(arr, &minVal, &maxVal);
            snprintf(buff, sizeof(buff), "[%lg,%lg]", minVal, maxVal);
            s += buff;
        }
       Logger::getInstance().log(s.c_str());

        // Show the actual data values
        printArray2D(M.data, M.cols, M.rows, channels, depth_type, M.step, maxElements);
    }
    else {
        LOG("%s empty Mat", s.c_str());
    }
}
    void printMatInfo(const cv::Mat M, const char *label)
    {
    printMat(M, label, -1);
    }
   // Print the label and then contents of a cvMat from the C interface (using "LOG()") for easy debugging.
   void printMatrix(const CvMat *M, const char *label, int maxElements)
{
    string s;
    char buff[32];
    if (label)
        s = label + string(": ");
    else
        s = "Matrix: ";
    if (M) {
        if (maxElements == 0)
            maxElements = M->rows * M->cols;
        sprintf(buff, "[%drows x %dcols]:\n", M->rows, M->cols);
        s += string(buff);
        Logger::getInstance().log(s.c_str());
        int channels = CV_MAT_CN(M->type);
        int depth = CV_MAT_DEPTH(M->type);
        int totalElements = 0;
        uchar *data = (uchar*)M->data.ptr;
        int step = M->step;

        for (int row=0; row < M->rows; row++) {
            string s = "";
            int element = 0;
            if (channels > 1 && M->rows > 1) {
                snprintf(buff, sizeof(buff), "row%d: ", row);
            }
            for (int col=0; col < M->cols; col++) {
                if (channels > 1)
                    s += "[";
                for (int ch=0; ch <= channels-1; ch++) {
                    if (ch > 0 || (channels == 1 && col != 0))    // Add a separator, except for the first element of this pixel or row.
                        s += ",";

                    buff[0] = '?';  // Initialize the string to "?" if something goes wrong.
                    buff[1] = 0;

                    // Allow to print just part of the image.
                    totalElements++;
                    if (totalElements > maxElements) {
                        // LOG can be printf or similar.
                        LOG("%s ... <just displaying the 1st %d entries from %d!>", s.c_str(), maxElements, M->rows * M->cols * channels);
                        return;
                    }

                    switch (depth) {
                    case CV_8U:
                    case CV_8S:         // 8-bit UCHAR Mat.
                        buff[0] = 'C';
                        //snprintf(buff, sizeof(buff), "%d", data[(row * step) + (col*channels) + ch]);
                        ; // UNTESTED!
                        break;
                    case CV_16U:
                    case CV_16S:   // 16-bit short Mat.
                        buff[0] = 'S';
                        //snprintf(buff, sizeof(buff), "%d", *(short*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(short)]);
                        ; // UNTESTED!
                        break;
                    case CV_32S:   // 32-bit int Mat.
                        buff[0] = 'I';
                        snprintf(buff, sizeof(buff), "%d", *(int*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(int)]);
                        ; // UNTESTED!
                        break;
                    case CV_32F:   // 32-bit float Mat.
                        snprintf(buff, sizeof(buff), "%.3f", *(float*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(float)]);
                        break;
                    case CV_64F:   // 64-bit double Mat.
                        buff[0] = 'D';
                        //snprintf(buff, sizeof(buff), "%.4lf", *(double*)(uchar*)&data[(row * step) + ((col*channels) + ch) * sizeof(double)]);
                        ; // UNTESTED!
                        break;
                    default:
                        snprintf(buff, sizeof(buff), "UNKNOWN DEPTH OF %d!", depth);
                    }
                    s += buff;

                    const int MAX_ELEMENTS_PER_LINE = 60;   // Only print upto 30 numbers per LOG() statement, since Android can only handle about 250 characters per log line!
                    if (element > MAX_ELEMENTS_PER_LINE) {
                        // LOG can be printf or similar.
                        Logger::getInstance().log(s.c_str());
                        s = "";
                        element = 0;
                    }
                    element++;
                }
                if (channels > 1)
                    s += "] ";
            }
            // LOG can be printf or similar.
            Logger::getInstance().log(s.c_str());
        }
    }
    else {
        LOG("[%s] = <null>!", s.c_str());
    }
}
    // where we assgin zero as default values 
    fisherfaceclassifer::fisherfaceclassifer(void)
	{
	// empty constrcutor	
	}
    // destructor 
    fisherfaceclassifer::~fisherfaceclassifer(void)
	{
	}
    // read the csv the prepared by python script to train the classifer
    void fisherfaceclassifer :: read_csv(const string& filename, char separator = ';') 
    {
    std::ifstream file(filename.c_str(), ifstream::in);
    if (!file) 
    {
    string error_message = "No valid input file was given, please check the given filename.";
    CV_Error(CV_StsBadArg, error_message);
    }
    int i=0;
    string line, path, classlabel;
    while (getline(file, line)) {
    stringstream liness(line);
    getline(liness, path, separator);
    getline(liness, classlabel);
    if(!path.empty() && !classlabel.empty()) 
    {
    Mat image;
    Mat imageProcessed,fface_resized;
    // Resize the image to be a consistent size, even if the aspect ratio changes.
    //imageProcessed = cvCreateImage(cvSize(180, 200), IPL_DEPTH_8U, 1);
    image = imread(path.c_str(),CV_LOAD_IMAGE_GRAYSCALE);   // Read the file
    // read image as gray scale image 
    // apply histogram equalization to avoid constrast and brightness problem
    equalizeHist(image, imageProcessed);
     //cv::resize(fface_resized, imageProcessed, Size(180, 200), 1.0, 1.0, INTER_CUBIC); 
    //csvimages.push_back(imread(path.c_str(),CV_LOAD_IMAGE_GRAYSCALE));
    csvimages.push_back(imageProcessed);
    labels.push_back(atoi(classlabel.c_str()));
//    namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
//    imshow( "Display window", image ); 
//   imshow( "Display window", imageProcessed );         // Show our image inside it.
    }
    }
   qDebug() << "Finish Reading";
}
    // free momory form dynamic pointer
    void fisherfaceclassifer::freeMemory(int fromIndex)
{
}
// train the dataset that I have (with to class male and famele) 
    bool fisherfaceclassifer::train()
{
string output_folder;
output_folder = "output";
string fn_csv ="Train/TrainingExamples.csv";
// Read in the data (fails if no valid input filename is given, but you'll get an error message):
try
{
qDebug() << "Read CSV images";
read_csv(fn_csv);
}
catch (cv::Exception& e) {
cerr << "Error opening file \"" << fn_csv << "\". Reason: " << e.msg << endl;
//nothing more we can do
exit(1);
}
    qDebug() << "Start training Images";
    // Get the height from the first image. We'll need this
    // later in code to reshape the images to their original
    // size AND we need to reshape incoming faces to this size:
    im_width = csvimages[0].cols;
    im_height = csvimages[0].rows;
    qDebug() << im_width;
    qDebug() << im_height;
    csvimages.pop_back();
    labels.pop_back();
    // Create a FaceRecognizer and train it on the given images:
    bool haveContribModule = initModule_contrib();
    if (!haveContribModule) {
    cerr << "ERROR: The 'contrib' module is needed for FaceRecognizer but has not been loaded into OpenCV!" << endl;
    exit(1);
    }
    model = createFisherFaceRecognizer(36);
    model->train(csvimages, labels);
    model->save("GenderClassify.xml");
   // showTrainingDebugData(model,im_width,im_height);
    isTrained = true;
    Mat mean = model->getMat("mean");
    // Display or save:
  //  imshow("mean", norm_0_255(mean.reshape(1, 200)));
    imwrite(format("%s/mean.png", output_folder.c_str()), norm_0_255(mean.reshape(1, 200)));
    // Display or save the first, at most 16 Fisherfaces:
    int height = 200;
    Mat eigenvalues = model->getMat("eigenvalues");
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat weigenvectors= model->getMat("eigenvectors");
    // Get the sample mean from the training data
    for (int i = 0; i < min(36, weigenvectors.cols); i++) {
    string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
    cout << msg << endl;
    // get eigenvector #i
    Mat ev = weigenvectors.col(i).clone();
    // Reshape to original size & normalize to [0...255] for imshow.
    Mat grayscale = norm_0_255(ev.reshape(1, height));
    // Show the image & apply a Bone colormap for better sensing.
    Mat cgrayscale;
    applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);
    // Display and save:
   // imshow(format("fisherface_%d", i), cgrayscale);
    imwrite(format("%s/fisherface_%d.png", output_folder.c_str(), i), norm_0_255(cgrayscale));
    }
    return 1; 
} 
    int fisherfaceclassifer::classdetection( cognition::Detector::RectVector &faces,
	    cv::Mat &frame,double &gconfidence)
{

   int predictionlab=-1;
   int identity=-1;
   string box_text;
   string output_folder;
   output_folder = "output";
   model->load("GenderClassify.xml");
   ///send faces and frame rect of all the faces appear in vedio webcam
    for(int i = 0; i < faces.size(); i++)
    {
   // Process face by face:
   // Find the faces in the frame:
   //Now we'll get the faces, make a prediction and
   // annotate it in the video.
   Mat original = frame.clone();
   // Convert the current frame to grayscale:
   Mat gray;
   for(int j=0; j<1000000000;j++)
   {
   }
   cvtColor(original, gray, CV_BGR2GRAY);
   Rect face_i = faces[i];
   // Crop the face from the image. So simple with OpenCV C++:
   Mat faced = gray(face_i);
   // Resizing the face is necessary for Eigenfaces and Fisherfaces. You can easily
   // verify this, by reading through the face recognition tutorial coming with OpenCV.
   // Resizing IS NOT NEEDED for Local Binary Patterns Histograms, so preparing the
    // input data really depends on the algorithm used.
    // face you have just found:
	Mat fface_resized,face_resized,imageProcessed;
	cv::resize(faced, fface_resized, Size(200, 220), 1.0, 1.0, INTER_CUBIC); 
	cv::Rect myROI(2,15, 180, 200);
	cv::Mat croppedImage;
     cv::Mat(fface_resized, myROI).copyTo(croppedImage)  ; 
	fface_resized=croppedImage;
     cv::resize(fface_resized, face_resized, Size(180, 200), 1.0, 1.0, INTER_CUBIC); 
     cvtColor(original, gray, CV_BGR2GRAY);
	//namedWindow( "Display window", CV_WINDOW_AUTOSIZE );// Create a window for display.
     equalizeHist(face_resized, imageProcessed);
	//imshow( "Display window", imageProcessed );// Show our image inside it.
     int predictlabal;
	double confidence;
	predictionlab=model->predict(imageProcessed);
	model->predict(imageProcessed,predictlabal,confidence);
	// And finally write all we've found out to the original image!
     // Create the text we will annotate the box with:
	qDebug() << "---- predict labal ----" << predictionlab;
	qDebug() << "---- predict labal----" << predictlabal;
	qDebug() << "---- confidence----" << confidence;
	if (predictionlab==1)
     box_text = format("Prediction = %s", "female");
	else 
     box_text = format("Prediction = %s", "male");
	// Generate a face approximation by back-projecting the eigenvectors & eigenvalues.
     Mat reconstructedFace;
     reconstructedFace = reconstructFace(model, imageProcessed);
     //imshow("reconstructedFace", reconstructedFace);
	imwrite(format("%s/fisherface_reconstruction_%d.png", output_folder.c_str(), 1), reconstructedFace);
     // Verify whether the reconstructed face looks like the preprocessed face, otherwise it is probably an unknown person.
     double similarity = getSimilarity(imageProcessed, reconstructedFace);
     string outputStr;
     if (similarity < 0.55f) 
	{
     // Identify who the person is in the preprocessed face image.
     identity = model->predict(imageProcessed);
     outputStr = toString(identity);
     }
     else 
	{
     // Since the confidence is low, assume it is an unknown person.
     outputStr = "Unknown";
     }
     cout << "Identity: " << outputStr << ". Similarity: " << similarity << endl;
     // Show the confidence rating for the recognition in the mid-top of the display.
      // Crop the confidence rating between 0.0 to 1.0, to show in the bar.
     double confidenceRatio = 1.0 - min(max(similarity, 0.0), 1.0);
     qDebug() << "FisherFace ConfidenceRatio" << confidenceRatio;
	gconfidence=confidenceRatio;
	qDebug() << "FisherFace ConfidenceRatio" << gconfidence;
	if(confidenceRatio<0.55)
	{
     predictionlab=-1;
	identity=-1;
	gconfidence=0;
	}Mat mean = model->getMat("mean");
     Mat eigenvalues = model->getMat("eigenvalues");
    // And we can do the same to display the Eigenvectors (read Eigenfaces):
    Mat weigenvectors= model->getMat("eigenvectors");
    for (int i = 0; i < min(16, weigenvectors.cols); i++) 
    {
        string msg = format("Eigenvalue #%d = %.5f", i, eigenvalues.at<double>(i));
        cout << msg << endl;
        // get eigenvector #i
        Mat ev = weigenvectors.col(i).clone();
        // Reshape to original size & normalize to [0...255] for imshow.
        Mat grayscale = norm_0_255(ev.reshape(1, 200));
        // Show the image & apply a Bone colormap for better sensing.
        Mat cgrayscale;
        applyColorMap(grayscale, cgrayscale, COLORMAP_BONE);       
    }
    for(int num_component = 0; num_component < min(16, weigenvectors.cols); num_component++) 
    {   // Slice the Fisherface from the model:
    Mat ev = weigenvectors.col(num_component);
    Mat projection = subspaceProject(ev, mean, csvimages[0].reshape(1,1));
    Mat reconstruction = subspaceReconstruct(ev, mean, projection);
    //Normalize the result:
   reconstruction = norm_0_255(reconstruction.reshape(1, csvimages[0].rows));
   // // Display and save:
 //  imshow(format("fisherface_reconstruction_%d", num_component), reconstruction);
   imwrite(format("%s/fisherface_reconstruction_%d.png", output_folder.c_str(), num_component), reconstruction);

    }
    }
   return predictionlab;  
 }
// Convert the matrix row or column (float matrix) to a rectangular 8-bit image that can be displayed or saved.
// Scales the values to be between 0 to 255.
Mat getImageFrom1DFloatMat(const Mat matrixRow, int height)
{
    // Make it a rectangular shaped image instead of a single row.
    Mat rectangularMat = matrixRow.reshape(1, height);
    // Scale the values to be between 0 to 255 and store them as a regular 8-bit uchar image.
    Mat dst;
    normalize(rectangularMat, dst, 0, 255, NORM_MINMAX, CV_8UC1);
    return dst;
}

// Show the internal face recognition data, to help debugging.
void fisherfaceclassifer :: showTrainingDebugData(const Ptr<FaceRecognizer> model,
const int faceWidth, const int faceHeight)
{
   string output_folder;
   output_folder = "output";
    try {   // Surround the OpenCV calls by a try/catch block so we don't crash if some model parameters aren't available.

        // Show the average face (statistical average for each pixel 
	   // in the collected images).
        Mat averageFaceRow = model->get<Mat>("mean");
        printMatInfo(averageFaceRow, "averageFaceRow");
        // Convert the matrix row (1D float matrix) to a regular 8-bit image.
        Mat averageFace = getImageFrom1DFloatMat(averageFaceRow, faceHeight);
        printMatInfo(averageFace, "averageFace");
      //  imshow("averageFace", averageFace);
	   imwrite(format("%s/averageFace.png", output_folder.c_str()), norm_0_255(averageFaceRow.reshape(1, 200)));
        // Get the eigenvectors
        Mat eigenvectors = model->get<Mat>("eigenvectors");
        printMatInfo(eigenvectors, "eigenvectors");
        // Show the best 36 eigenfaces
        for (int i = 0; i < min(36, eigenvectors.cols); i++) {
        // Create a column vector from eigenvector #i.
        // Note that clone() ensures it will be continuous, so we can treat it like an array, otherwise we can't reshape it to a rectangle.
        // Note that the FaceRecognizer class already gives us L2 normalized eigenvectors, so we don't have to normalize them ourselves.
        Mat eigenvectorColumn = eigenvectors.col(i).clone();
        //printMatInfo(eigenvectorColumn, "eigenvector");
        Mat eigenface = getImageFrom1DFloatMat(eigenvectorColumn, faceHeight);
        //printMatInfo(eigenface, "eigenface");
    //    imshow(format("Eigenface%d", i), eigenface);
	   imwrite(format("%s/Eigenface.png", output_folder.c_str()), norm_0_255(averageFaceRow.reshape(1, 200)));
        }

        // Get the eigenvalues
        Mat eigenvalues = model->get<Mat>("eigenvalues");
        printMat(eigenvalues, "eigenvalues",0);
        int ncomponents = model->get<int>("ncomponents");
        cout << "ncomponents = " << ncomponents << endl;

        vector<Mat> projections = model->get<vector<Mat> >("projections");
        cout << "projections: " << projections.size() << endl;
        for (int i = 0; i < (int)projections.size(); i++) {
            printMat(projections[i], "projections",0);
        }

      

    } catch (cv::Exception e) {
        //cout << "WARNING: Missing FaceRecognizer properties." << endl;
    }

}
// Compare two images by getting the L2 error (square-root of sum of squared error).
// Generate an approximately reconstructed face by back-projecting the eigenvectors & eigenvalues of the given (preprocessed) face.
Mat fisherfaceclassifer :: reconstructFace(const Ptr<FaceRecognizer> model, const Mat preprocessedFace)
{
    // Since we can only reconstruct the face for some types of FaceRecognizer models (ie: Eigenfaces or Fisherfaces),
    // we should surround the OpenCV calls by a try/catch block so we don't crash for other models.
    try {

        // Get some required data from the FaceRecognizer model.
        Mat eigenvectors = model->get<Mat>("eigenvectors");
        Mat averageFaceRow = model->get<Mat>("mean");

        int faceHeight = preprocessedFace.rows;

        // Project the input image onto the PCA subspace.
        Mat projection = subspaceProject(eigenvectors, averageFaceRow, preprocessedFace.reshape(1,1));
        //printMatInfo(projection, "projection");

        // Generate the reconstructed face back from the PCA subspace.
        Mat reconstructionRow = subspaceReconstruct(eigenvectors, averageFaceRow, projection);
        //printMatInfo(reconstructionRow, "reconstructionRow");

        // Convert the float row matrix to a regular 8-bit image. Note that we
        // shouldn't use "getImageFrom1DFloatMat()" because we don't want to normalize
        // the data since it is already at the perfect scale.

        // Make it a rectangular shaped image instead of a single row.
        Mat reconstructionMat = reconstructionRow.reshape(1, faceHeight);
        // Convert the floating-point pixels to regular 8-bit uchar pixels.
        Mat reconstructedFace = Mat(reconstructionMat.size(), CV_8U);
        reconstructionMat.convertTo(reconstructedFace, CV_8U, 1, 0);
        //printMatInfo(reconstructedFace, "reconstructedFace");

        return reconstructedFace;

    } catch (cv::Exception e) {
        //cout << "WARNING: Missing FaceRecognizer properties." << endl;
        return Mat();
    }
}
double fisherfaceclassifer :: getSimilarity(const Mat A, const Mat B)
{
    if (A.rows > 0 && A.rows == B.rows && A.cols > 0 && A.cols == B.cols) {
        // Calculate the L2 relative error between the 2 images.
        double errorL2 = norm(A, B, CV_L2);
        // Convert to a reasonable scale, since L2 error is summed across all pixels of the image.
        double similarity = errorL2 / (double)(A.rows * A.cols);
        return similarity;
    }
    else {
        //cout << "WARNING: Images have a different size in 'getSimilarity()'." << endl;
        return 100000000.0;  // Return a bad value
    }
}

}