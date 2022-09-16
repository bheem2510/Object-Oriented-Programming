#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<cmath>
#include<iostream>
#include<conio.h>           // it may be necessary to change or remove this line if not using Windows

#include "Blob.h"
using namespace cv;
using namespace std;

#define SHOW_STEPS            // un-comment or comment this line to show steps or not
/* creating colours*/
// global variables ///////////////////////////////////////////////////////////////////////////////
const cv::Scalar SCALAR_BLACK = cv::Scalar(0.0, 0.0, 0.0);
const cv::Scalar SCALAR_WHITE = cv::Scalar(255.0, 255.0, 255.0);
const cv::Scalar SCALAR_YELLOW = cv::Scalar(0.0, 255.0, 255.0);
const cv::Scalar SCALAR_GREEN = cv::Scalar(0.0, 200.0, 0.0);
const cv::Scalar SCALAR_RED = cv::Scalar(0.0, 0.0, 255.0);
int drawattemptcount = 0;

// function prototypes ////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs);
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex);
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs);
double distanceBetweenPoints(cv::Point point1, cv::Point point2);
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName);
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName);
/*bottom lane*/
bool checkIfBlobsCrossedThebottomtotopLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition1, int &intHorizontalLinePosition2, int &carCount);
bool checkIfBlobsCrossedThebottomtoleftLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition2, int &leftcarCount);
bool checkIfBlobsCrossedThebottomtorightLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition2, int &rightcarCount);
/*top lane*/
bool checkIfBlobsCrossedThetoptobottomLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition1, int &intHorizontalLinePosition2, int &carCount);
bool checkIfBlobsCrossedThetoptorightLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition1, int &toprcarCount);
bool checkIfBlobsCrossedThetoptoleftLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition1, int &toplcarcount);
/*left lane*/
bool checkIfBlobsCrossedThelefttorightLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intVerticalLinePositionright, int &leftrcarCount);
bool checkIfBlobsCrossedThelefttobottomtLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition2, int &leftbcarCount);
bool checkIfBlobsCrossedThelefttotopLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition1, int &lefttcarcount);
/*right lane*/
bool checkIfBlobsCrossedTherighttoleftLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intVerticalLinePositionright, int &rightlcarCount);
bool checkIfBlobsCrossedTherighttobottomtLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition2, int &rightbcarCount);
bool checkIfBlobsCrossedTherighttotopLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition1, int &righttcarcount);


void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy);
void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy,int &yoffset,int &xoffset,string &directionName);
///////////////////////////////////////////////////////////////////////////////////////////////////

class flags{
public:
bool flagforwardbottomtotop ;
bool flagbottomtoleft ;
bool flagbottomtoright;
bool flagtoptobottom;
bool flagtoptoleft;
bool flagtoptoright;
bool flaglefttoright;
bool flaglefttobottom;
bool flaglefttotop;
bool flagrighttoleft;
bool flagrighttobottom;
bool flagrighttotop;
}Flags[500];

unsigned int z = 0;

int main(void) {

	cv::VideoCapture play;
	cv::Mat frame, src1, src2, dst;                                                           //creating image type variables
	cv::Rect r(0, 450, 520, 269), r1(750, 0, 528, 240), r3(805, 365, 475, 355),r4(1100,350,178,50), r2(0, 0, 550, 250),r5(0,250,500,10); //creating region of interests 
	 
	cv::Mat imgFrame1;
	cv::Mat imgFrame2;
	string directionName;
	int yoffset,xoffset;
	std::vector<Blob> blobs;
	/*horizontal lines top and bottom*/
	cv::Point crossingLine1[2];     // top line coordinates horizontal line-1
	cv::Point crossingLine2[2];     // bottom line coordinates horizontal line-2
	/*vertical lines left and right*/
	cv::Point crossingLine3[2];     // top line coordinates horizontal line-1
	cv::Point crossingLine4[2];     // bottom line coordinates horizontal line-2
	
	int carCount = 0, leftcarCount = 0, rightcarCount = 0, topbcarCount = 0, toprcarCount = 0, toplcarCount = 0, leftrcarCount = 0,
		leftbcarCount = 0, lefttcarCount = 0, rightlcarCount = 0, rightbcarCount = 0, righttcarcount = 0;

	play.open("CarsDrivingUnderBridge.mp4");

	if (!play.isOpened()) {                                                 // if unable to open video file
		std::cout << "error reading video file" << std::endl << std::endl;      // show error message
		_getch();                   // it may be necessary to change or remove this line if not using Windows
		return(0);                                                              // and exit program
	}

	if (play.get(CV_CAP_PROP_FRAME_COUNT) < 2) {
		std::cout << "error: video file must have at least two frames";
		_getch();                   // it may be necessary to change or remove this line if not using Windows
		return(0);
	}

	play.read(imgFrame1);
    play.read(imgFrame2);
	
	/*horizontal lines*/
	int intHorizontalLinePosition1 = 100;//(int)std::round((double)imgFrame1.rows * 0.35);
	int intHorizontalLinePosition2 = 500;//(int)std::round((double)imgFrame1.rows * 0.35);

	//std::cout <<"horizontal line position = "<< intHorizontalLinePosition1<<endl;
	//std::cout << "horizontal line position = " << intHorizontalLinePosition2 << endl;
	/*line1 starting and ending coordinates*/                          /*Line Positions*/
	crossingLine1[0].x = 580;//0                                         
	crossingLine1[0].y = intHorizontalLinePosition1;                   

	crossingLine1[1].x = 725;//imgFrame1.cols - 1;
	crossingLine1[1].y = intHorizontalLinePosition1;

	/*line2 starting and ending coordinates*/
	crossingLine2[0].x = 550;//0
	crossingLine2[0].y = intHorizontalLinePosition2;

	crossingLine2[1].x = 775;//imgFrame1.cols - 1;
	crossingLine2[1].y = intHorizontalLinePosition2;

	/*vertical lines*/
	int intVerticalLinePositionleft = 450;
	int intVerticalLinePositionright = 850;
	/*point 1 left line*/
	crossingLine3[0].x = intVerticalLinePositionleft;
	crossingLine3[0].y = 450;
	/*point 2 left line*/
	crossingLine3[1].x = intVerticalLinePositionleft;
	crossingLine3[1].y = 275;
	/*point 1 right line*/
	crossingLine4[0].x = intVerticalLinePositionright;
	crossingLine4[0].y = 360;
	/*point 2 right line*/
	crossingLine4[1].x = intVerticalLinePositionright;
	crossingLine4[1].y = 275;

	char chCheckForEscKey = 0;

	bool blnFirstFrame = true;

	int frameCount = 2;
	int numberoftimesitisentering = 1;
	while (play.isOpened() && chCheckForEscKey != 27) {
		numberoftimesitisentering++;
		
			//cout << "hi " << endl;
			play >> imgFrame2;                                                                  //getting the next frame from the video
			//imshow("frame", frame);																//showing the original frame
			if (((numberoftimesitisentering) % 3 ) == 0) {																					/*croping the required part of the image in this section*/
			imgFrame2(r2) = 0;//left top
			imgFrame2(r5) = 0;//sligt bottom left top
			imgFrame2(r) = 0;//left bottom
			imgFrame2(r1) = 0;//right top'
			imgFrame2(r3) = 0;//right bottom
			imgFrame2(r4) = 0;//Slight up right bottom
						  /*Uptil here*/
			std::vector<Blob> currentFrameBlobs;

			cv::Mat imgFrame1Copy = imgFrame1.clone();
			cv::Mat imgFrame2Copy = imgFrame2.clone();

			cv::Mat imgDifference;
			cv::Mat imgThresh;

			cv::cvtColor(imgFrame1Copy, imgFrame1Copy, CV_BGR2GRAY);
			cv::cvtColor(imgFrame2Copy, imgFrame2Copy, CV_BGR2GRAY);

			cv::GaussianBlur(imgFrame1Copy, imgFrame1Copy, cv::Size(5, 5), 0);
			cv::GaussianBlur(imgFrame2Copy, imgFrame2Copy, cv::Size(5, 5), 0);
			
			cv::absdiff(imgFrame1Copy, imgFrame2Copy, imgDifference);
			imshow("hello", imgDifference);
			cv::threshold(imgDifference, imgThresh, 20, 255.0, CV_THRESH_BINARY);/// responsible for detection of single blob into multiple have to correct this

			cv::imshow("imgThresh", imgThresh);

			cv::Mat structuringElement3x3 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
			cv::Mat structuringElement5x5 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
			cv::Mat structuringElement7x7 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));
			cv::Mat structuringElement15x15 = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));

			for (unsigned int i = 0; i < 2; i++) {
				cv::dilate(imgThresh, imgThresh, structuringElement5x5);
				cv::dilate(imgThresh, imgThresh, structuringElement5x5);
				cv::erode(imgThresh, imgThresh, structuringElement5x5);
			}

			cv::Mat imgThreshCopy = imgThresh.clone();

			std::vector<std::vector<cv::Point> > contours;

			cv::findContours(imgThreshCopy, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
			
			//cout << endl << "contour = " << contours[0] << endl;
			drawAndShowContours(imgThresh.size(), contours, "imgContours");

			std::vector<std::vector<cv::Point> > convexHulls(contours.size());

			for (unsigned int i = 0; i < contours.size(); i++) {
				cv::convexHull(contours[i], convexHulls[i]);
			}

			drawAndShowContours(imgThresh.size(), convexHulls, "imgConvexHulls");

			for (auto &convexHull : convexHulls) {
				Blob possibleBlob(convexHull);

				if (possibleBlob.currentBoundingRect.area() > 400 &&
					possibleBlob.dblCurrentAspectRatio > 0.2 &&
					possibleBlob.dblCurrentAspectRatio < 4.0 &&
					possibleBlob.currentBoundingRect.width > 30 &&
					possibleBlob.currentBoundingRect.height > 30 &&
					possibleBlob.dblCurrentDiagonalSize > 60.0 &&
					(cv::contourArea(possibleBlob.currentContour) / (double)possibleBlob.currentBoundingRect.area()) > 0.50) {
					currentFrameBlobs.push_back(possibleBlob);
				}
			}

			drawAndShowContours(imgThresh.size(), currentFrameBlobs, "imgCurrentFrameBlobs");

			if (blnFirstFrame == true) {
				for (auto &currentFrameBlob : currentFrameBlobs) {
					blobs.push_back(currentFrameBlob);
				}
			}
			else {
				matchCurrentFrameBlobsToExistingBlobs(blobs, currentFrameBlobs);
			}

			drawAndShowContours(imgThresh.size(), blobs, "imgBlobs");
			//imshow("hiii..", blobs[0]);
			//cout << "blob" << blobs[0] << endl;

			imgFrame2Copy = imgFrame2.clone();          // get another copy of frame 2 since we changed the previous frame 2 copy in the processing above

			drawBlobInfoOnImage(blobs, imgFrame2Copy);
			/*bottom to top*/
			bool blnAtLeastOneBlobCrossedThebottomtotopLine = checkIfBlobsCrossedThebottomtotopLine(blobs, intHorizontalLinePosition1, intHorizontalLinePosition2, carCount);
			/*bottom to left*/
			bool blnAtLeastOneBlobCrossedThebottomtoleftLine = checkIfBlobsCrossedThebottomtoleftLine(blobs, intVerticalLinePositionleft, intHorizontalLinePosition2, leftcarCount);
			/*left vertical line*/
			if (blnAtLeastOneBlobCrossedThebottomtoleftLine == true) {
				cv::line(imgFrame2Copy, crossingLine3[0], crossingLine3[1], SCALAR_GREEN, 2);
			}
			else {
				cv::line(imgFrame2Copy, crossingLine3[0], crossingLine3[1], SCALAR_RED, 2);
			}
			/*bottom to right*/
			bool blnAtLeastOneBlobCrossedThebottomtorightLine = checkIfBlobsCrossedThebottomtorightLine(blobs, intVerticalLinePositionright, intHorizontalLinePosition2, rightcarCount);
			/*right vertical line*/
			if (blnAtLeastOneBlobCrossedThebottomtorightLine == true) {
				cv::line(imgFrame2Copy, crossingLine4[0], crossingLine4[1], SCALAR_GREEN, 2);
			}
			else {
				cv::line(imgFrame2Copy, crossingLine4[0], crossingLine4[1], SCALAR_RED, 2);
			}
			////////////////////////////////////////////
			/*top to bottom*/
			bool blnAtLeastOneBlobCrossedThetoptobottomLine = checkIfBlobsCrossedThetoptobottomLine(blobs, intHorizontalLinePosition1, intHorizontalLinePosition2, topbcarCount);
			/*top to right*/
			bool blnAtLeastOneBlobCrossedThetoptorightLine = checkIfBlobsCrossedThetoptorightLine(blobs, intVerticalLinePositionleft, intHorizontalLinePosition1, toprcarCount);
			/*top to left*/
			bool blnAtLeastOneBlobCrossedThetoptoleftLine = checkIfBlobsCrossedThetoptoleftLine(blobs, intVerticalLinePositionright, intHorizontalLinePosition1, toplcarCount);
			/////////////////////////////////////////////
			/*left to right*/
			bool blnAtLeastOneBlobCrossedThelefttorightLine = checkIfBlobsCrossedThelefttorightLine(blobs, intVerticalLinePositionleft, intVerticalLinePositionright, leftrcarCount);
			/*left to bottom*/
			bool blnAtLeastOneBlobCrossedThelefttobottomLine = checkIfBlobsCrossedThelefttobottomtLine(blobs, intVerticalLinePositionleft, intHorizontalLinePosition2, leftbcarCount);
			/*left to top*/
			bool blnAtLeastOneBlobCrossedThelefttotopLine = checkIfBlobsCrossedThelefttotopLine(blobs, intVerticalLinePositionleft, intHorizontalLinePosition1, lefttcarCount);
			//////////////////////////////////////////
			/*right to left*/
			bool blnAtLeastOneBlobCrossedTherighttoleftLine = checkIfBlobsCrossedTherighttoleftLine(blobs, intVerticalLinePositionleft, intVerticalLinePositionright, rightlcarCount);
			/*right to bottom*/
			bool blnAtLeastOneBlobCrossedTherighttobottomLine = checkIfBlobsCrossedTherighttobottomtLine(blobs, intVerticalLinePositionright, intHorizontalLinePosition2, rightbcarCount);
			/*right to top*/
			bool blnAtLeastOneBlobCrossedTherighttotopLine = checkIfBlobsCrossedTherighttotopLine(blobs, intVerticalLinePositionright, intHorizontalLinePosition1, righttcarcount);
			/*ends here*/
			/*top horizontal line 1*/
			if ((blnAtLeastOneBlobCrossedThebottomtotopLine == true) || (blnAtLeastOneBlobCrossedThetoptobottomLine == true)) {
				cv::line(imgFrame2Copy, crossingLine1[0], crossingLine1[1], SCALAR_GREEN, 2); // this is the 1st line top forward line
			}
			else {
				cv::line(imgFrame2Copy, crossingLine1[0], crossingLine1[1], SCALAR_RED, 2);
			}
			/*added for bottom horizontal line 2*/
			if ((blnAtLeastOneBlobCrossedThebottomtotopLine == true)||(blnAtLeastOneBlobCrossedThetoptobottomLine == true)) {
				cv::line(imgFrame2Copy, crossingLine2[0], crossingLine2[1], SCALAR_GREEN, 2);
			}
			else {
				cv::line(imgFrame2Copy, crossingLine2[0], crossingLine2[1], SCALAR_RED, 2);
			}
			/*bottom to top*/
			directionName = "forward:";
			yoffset = 400 + 0;
			xoffset = 0;
			drawCarCountOnImage(carCount, imgFrame2Copy, yoffset, xoffset, directionName);
			directionName = "left:";
			yoffset = 400 + 50;
			xoffset = 0;
			//int off =400+ 50;
			drawCarCountOnImage(leftcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			directionName = "right:";
			yoffset = 400 + 100;
			xoffset = 0;
			//int off1 = 400+100;
			drawCarCountOnImage(rightcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*top to bottom*/
			directionName = "straight:";
			yoffset = 0;
			xoffset = 800;
			drawCarCountOnImage(topbcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*top to right*/
			directionName = "right:";
			yoffset = 50;
			xoffset = 800;
			drawCarCountOnImage(toprcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*top to left*/
			directionName = "left:";
			yoffset = 100;
			xoffset = 800;
			drawCarCountOnImage(toplcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*left to right*/
			directionName = "straight:";
			yoffset = 450;
			xoffset = 800;
			drawCarCountOnImage(leftrcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*left to bottom*/
			directionName = "right:";
			yoffset = 500;
			xoffset = 800;
			drawCarCountOnImage(leftbcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*left to top*/
			directionName = "left:";
			yoffset = 550;
			xoffset = 800;
			drawCarCountOnImage(lefttcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/*right to left*/
			directionName = "straight:";
			yoffset = 0;
			xoffset = 50;
			drawCarCountOnImage(rightlcarCount, imgFrame2Copy, yoffset, xoffset, directionName);

			/*right to bottom*/
			directionName = " left:";
			yoffset = 50;
			xoffset = 50;
			drawCarCountOnImage(rightbcarCount, imgFrame2Copy, yoffset, xoffset, directionName);
			/* right to top*/
			directionName = " right:";
			yoffset = 95;
			xoffset = 50;
			drawCarCountOnImage(righttcarcount, imgFrame2Copy, yoffset, xoffset, directionName);

			cv::imshow("imgFrame2Copy", imgFrame2Copy);
			//cv::waitKey(0);                 // uncomment this line to go frame by frame for debugging

			// now we prepare for the next iteration

			currentFrameBlobs.clear();

			imgFrame1 = imgFrame2.clone();           // move frame 1 up to where frame 2 is

			if ((play.get(CV_CAP_PROP_POS_FRAMES) + 1) < play.get(CV_CAP_PROP_FRAME_COUNT)) {
				play.read(imgFrame2);
			}
			else {
				std::cout << "end of video\n";
				break;
			}

			blnFirstFrame = false;
			frameCount++;
			chCheckForEscKey = cv::waitKey(1);
		}
	}
	

	if (chCheckForEscKey != 27) {               // if the user did not press esc (i.e. we reached the end of the video)
		cv::waitKey(0);                         // hold the windows open to allow the "end of video" message to show
	}

	// note that if the user did press esc, we don't need to hold the windows open, we can simply let the program end which will close the windows

	return(0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void matchCurrentFrameBlobsToExistingBlobs(std::vector<Blob> &existingBlobs, std::vector<Blob> &currentFrameBlobs) {

	for (auto &existingBlob : existingBlobs) {

		existingBlob.blnCurrentMatchFoundOrNewBlob = false;

		existingBlob.predictNextPosition();
	}

	for (auto &currentFrameBlob : currentFrameBlobs) {

		int intIndexOfLeastDistance = 0;
		double dblLeastDistance = 100000.0;

		for (unsigned int i = 0; i < existingBlobs.size(); i++) {

			if (existingBlobs[i].blnStillBeingTracked == true) {

				double dblDistance = distanceBetweenPoints(currentFrameBlob.centerPositions.back(), existingBlobs[i].predictedNextPosition);

				if (dblDistance < dblLeastDistance) {
					dblLeastDistance = dblDistance;
					intIndexOfLeastDistance = i;
				}
			}
		}

		if (dblLeastDistance < currentFrameBlob.dblCurrentDiagonalSize * 0.5) {
			addBlobToExistingBlobs(currentFrameBlob, existingBlobs, intIndexOfLeastDistance);
		}
		else {
			addNewBlob(currentFrameBlob, existingBlobs);
		}

	}

	for (auto &existingBlob : existingBlobs) {

		if (existingBlob.blnCurrentMatchFoundOrNewBlob == false) {
			existingBlob.intNumOfConsecutiveFramesWithoutAMatch++;
		}

		if (existingBlob.intNumOfConsecutiveFramesWithoutAMatch >= 5) {
			existingBlob.blnStillBeingTracked = false;
		}

	}

}
//=================================================================================================
///////////////////////////////////////////////////////////////////////////////////////////////////
void addBlobToExistingBlobs(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs, int &intIndex) {

	existingBlobs[intIndex].currentContour = currentFrameBlob.currentContour;
	existingBlobs[intIndex].currentBoundingRect = currentFrameBlob.currentBoundingRect;

	existingBlobs[intIndex].centerPositions.push_back(currentFrameBlob.centerPositions.back());

	existingBlobs[intIndex].dblCurrentDiagonalSize = currentFrameBlob.dblCurrentDiagonalSize;
	existingBlobs[intIndex].dblCurrentAspectRatio = currentFrameBlob.dblCurrentAspectRatio;

	existingBlobs[intIndex].blnStillBeingTracked = true;
	existingBlobs[intIndex].blnCurrentMatchFoundOrNewBlob = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void addNewBlob(Blob &currentFrameBlob, std::vector<Blob> &existingBlobs) {

	currentFrameBlob.blnCurrentMatchFoundOrNewBlob = true;

	existingBlobs.push_back(currentFrameBlob);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
double distanceBetweenPoints(cv::Point point1, cv::Point point2) {

	int intX = abs(point1.x - point2.x);
	int intY = abs(point1.y - point2.y);

	return(sqrt(pow(intX, 2) + pow(intY, 2)));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<std::vector<cv::Point> > contours, std::string strImageName) {
	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawAndShowContours(cv::Size imageSize, std::vector<Blob> blobs, std::string strImageName) {

	cv::Mat image(imageSize, CV_8UC3, SCALAR_BLACK);

	std::vector<std::vector<cv::Point> > contours;

	for (auto &blob : blobs) {
		if (blob.blnStillBeingTracked == true) {
			contours.push_back(blob.currentContour);
		}
	}

	cv::drawContours(image, contours, -1, SCALAR_WHITE, -1);

	cv::imshow(strImageName, image);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/*changed in the if statement and also in the function arguments*///
bool checkIfBlobsCrossedThebottomtotopLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition1, int &intHorizontalLinePosition2, int &carCount) {
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	
	//for (unsigned int i = 0; i < blobs.size(); i++) {
		int blobnumber = blobs.size()-1;
		int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			if (presentcarcount != 11) {
				int prevFrameIndex = (int)blob.centerPositions.size() - 2;
				int currFrameIndex = (int)blob.centerPositions.size() - 1;
				//cout << "the car number is  " <<presentcarcount-5<< endl;
				//blobs[blobnumber].show(blobnumber);
				cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
				cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
				if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition2 && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition2)
				{
					Flags[presentcarcount].flagforwardbottomtotop = true;
					/*cout << "phase-1" << endl;
					cout << "phase-1" << endl;
					cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
					cout << "phase-1" << endl;*/


				}
				if ((Flags[presentcarcount].flagforwardbottomtotop) && (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition1 && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition1))
				{

					carCount++;
					blnAtLeastOneBlobCrossedTheLine = true;
					Flags[presentcarcount].flagforwardbottomtotop = false;
					Flags[presentcarcount].flagbottomtoleft = false;
					//break;
				}


				//cout << "hello" << endl;


			}
		}

	}

	return blnAtLeastOneBlobCrossedTheLine;
}


//////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedThebottomtoleftLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition2, int &leftcarCount) {
	bool blnAtLeastOneBlobCrossedTheLine = false;
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	for (auto blob : blobs) {
		presentcarcount++;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "centerpostiton x = " << blob.centerPositions[prevFrameIndex].x << endl;

			if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition2 && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition2)
			{
				Flags[presentcarcount].flagforwardbottomtotop = true;
				Flags[presentcarcount].flagbottomtoleft = true;
				/*cout << "phase-2" << endl;
				cout << "phase-2" << endl;
				cout << "phase-2" << endl; cout << "phase-2 " << endl; 
				cout << "phase-2" << endl;
				*/

			}
			if ((Flags[presentcarcount].flagbottomtoleft) && (blob.centerPositions[prevFrameIndex].x > intVerticalLinePositionleft && blob.centerPositions[currFrameIndex].x <= intVerticalLinePositionleft))
			{

				leftcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagbottomtoleft = false;
				//break;
			}

			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;




}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
bool checkIfBlobsCrossedThebottomtorightLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition2, int &rightcarCount) {
	bool blnAtLeastOneBlobCrossedTheLine = false;
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	for (auto blob : blobs) {
		presentcarcount++;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "centerpostiton x for right= " << blob.centerPositions[prevFrameIndex].x << endl;

			if (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition2 && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition2)
			{
				Flags[presentcarcount].flagforwardbottomtotop = true;
				Flags[presentcarcount].flagbottomtoright = true;
				/*cout << "phase-2" << endl;
				cout << "phase-2" << endl;
				cout << "phase-2" << endl; cout << "phase-2 " << endl;
				cout << "phase-2" << endl;
				*/

			}
			if ((Flags[presentcarcount].flagbottomtoright) && (blob.centerPositions[prevFrameIndex].x > intVerticalLinePositionright && blob.centerPositions[currFrameIndex].x <= intVerticalLinePositionright))
			{

				rightcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagbottomtoright = false;
				//break;
			}

			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;




}

/*******Top to bottom (Reverse direction) *******/
bool checkIfBlobsCrossedThetoptobottomLine(std::vector<Blob> &blobs, int &intHorizontalLinePosition1, int &intHorizontalLinePosition2, int &topbcarcount) {
	bool blnAtLeastOneBlobCrossedTheLine = false;
	cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			//cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
			if (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition1 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition1)
			{
				Flags[presentcarcount].flagtoptobottom = true;
				Flags[presentcarcount].flagtoptoleft = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flagtoptobottom) && (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition2 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition2))
			{

				topbcarcount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagtoptobottom = false;
				Flags[presentcarcount].flagbottomtoleft = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;
}
/*top to right(left becomes right)*/
bool checkIfBlobsCrossedThetoptorightLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition1, int &toprcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	for (auto blob : blobs) {
		presentcarcount++;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "centerpostiton x = " << blob.centerPositions[prevFrameIndex].x << endl;

			if (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition1 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition1)
			{
				Flags[presentcarcount].flagtoptobottom = true;
				Flags[presentcarcount].flagtoptoright = true;
				/*cout << "phase-2" << endl;
				cout << "phase-2" << endl;
				cout << "phase-2" << endl; cout << "phase-2 " << endl;
				cout << "phase-2" << endl;
				*/

			}
			if ((Flags[presentcarcount].flagtoptoright) && (blob.centerPositions[prevFrameIndex].x > intVerticalLinePositionleft && blob.centerPositions[currFrameIndex].x <= intVerticalLinePositionleft))
			{

				toprcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagtoptoright = false;
				//break;
			}

			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;




}
/*top to left*/
bool checkIfBlobsCrossedThetoptoleftLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition1, int &toplcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	for (auto blob : blobs) {
		presentcarcount++;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "centerpostiton x for right= " << blob.centerPositions[prevFrameIndex].x << endl;

			if (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition1 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition1)
			{
				Flags[presentcarcount].flagtoptoleft = true;
				Flags[presentcarcount].flagtoptobottom = true;
				/*cout << "phase-2" << endl;
				cout << "phase-2" << endl;
				cout << "phase-2" << endl; cout << "phase-2 " << endl;
				cout << "phase-2" << endl;
				*/

			}
			if ((Flags[presentcarcount].flagbottomtoleft) && (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionright && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionright))
			{

				toplcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagtoptoleft = false;
				//break;
			}

			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;





}
/*left to right*/
bool checkIfBlobsCrossedThelefttorightLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intVerticalLinePositionright, int &leftrcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		cout << "car count -----+++*** " << presentcarcount << endl;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "you have entered the show() here " << endl;
			//blobs[blobnumber].show(blobnumber);
			//cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			//cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
			if (presentcarcount == 44 ) {
				presentcarcount = 37;//Flags[presentcarcount].flaglefttoright = true; 
			}
			if (presentcarcount == 56) { Flags[presentcarcount].flaglefttoright = true; }
			if (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionleft && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionleft)
			{
				cout << "car count " << presentcarcount << endl;
				Flags[presentcarcount].flaglefttoright = true;
				Flags[presentcarcount].flaglefttobottom = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flaglefttoright) && (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionright && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionright))
			{

				leftrcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flaglefttoright = false;
				Flags[presentcarcount].flaglefttobottom = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;

}
/*left to bottom*/
bool checkIfBlobsCrossedThelefttobottomtLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition2, int &leftbcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "you have entered the show() here " << endl;
			//blobs[blobnumber].show(blobnumber);
			//cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			//cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
			if (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionleft && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionleft)
			{
				Flags[presentcarcount].flaglefttoright = true;
				Flags[presentcarcount].flaglefttobottom = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flaglefttobottom) && (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition2 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition2))
			{

				leftbcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flaglefttoright = false;
				Flags[presentcarcount].flaglefttobottom = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;


}
/*left to top*/
bool checkIfBlobsCrossedThelefttotopLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intHorizontalLinePosition1, int &lefttcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			//cout << "you have entered the show() here " << endl;
			//blobs[blobnumber].show(blobnumber);
			//cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			//cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
			if (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionleft && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionleft)
			{
				Flags[presentcarcount].flaglefttotop = true;
				Flags[presentcarcount].flaglefttobottom = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flaglefttotop) && (blob.centerPositions[prevFrameIndex].y > intHorizontalLinePosition1 && blob.centerPositions[currFrameIndex].y <= intHorizontalLinePosition1))
			{

				lefttcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flaglefttoright = false;
				Flags[presentcarcount].flaglefttobottom = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;


}

/*right to left*/
bool checkIfBlobsCrossedTherighttoleftLine(std::vector<Blob> &blobs, int &intVerticalLinePositionleft, int &intVerticalLinePositionright, int &rightlcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
			/*cout << "you have entered the show() here " << endl;
			blobs[blobnumber].show(blobnumber);
			cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;*/
			if (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionright && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionright)
			{
				Flags[presentcarcount].flagrighttoleft = true;
				//Flags[presentcarcount].flaglefttobottom = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flagrighttoleft) && (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionleft && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionleft))
			{

				rightlcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagrighttoleft = false;
				//[presentcarcount].flaglefttobottom = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;

}
/*right to bottom*/
bool checkIfBlobsCrossedTherighttobottomtLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition2, int &rightbcarCount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
		//	cout << "you have entered the show() here " << endl;
			//blobs[blobnumber].show(blobnumber);
			//cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			//cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
			if (blob.centerPositions[prevFrameIndex].x > intVerticalLinePositionright && blob.centerPositions[currFrameIndex].x <= intVerticalLinePositionright)
			{
				//Flags[presentcarcount].flaglefttoright = true;
				Flags[presentcarcount].flagrighttobottom = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flagrighttobottom) && (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition2 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition2))
			{

				rightbcarCount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				//Flags[presentcarcount].flaglefttoright = false;
				Flags[presentcarcount].flagrighttobottom = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;


}

/*right to top*/
bool checkIfBlobsCrossedTherighttotopLine(std::vector<Blob> &blobs, int &intVerticalLinePositionright, int &intHorizontalLinePosition1, int &righttcarcount)
{
	bool blnAtLeastOneBlobCrossedTheLine = false;
	//cout << "blob size in check block = " << blobs.size() << endl;
	//numberoftimesitisentering++;
	//for (unsigned int i = 0; i < blobs.size(); i++) {
	int blobnumber = blobs.size() - 1;
	int presentcarcount = 0;
	//}
	//cout << "number of times it is entering = " << numberoftimesitisentering << endl;
	for (auto blob : blobs) {
		presentcarcount++;
		//cout << "number of times entering into the fo loop" << op << endl;
		if (blob.blnStillBeingTracked == true && blob.centerPositions.size() >= 2) {
			int prevFrameIndex = (int)blob.centerPositions.size() - 2;
			int currFrameIndex = (int)blob.centerPositions.size() - 1;
		//	cout << "you have entered the show() here " << endl;
			//blobs[blobnumber].show(blobnumber);
			//cout << "1st block centerpostiton y = " << blob.centerPositions[prevFrameIndex].y << endl;
			//cout << "1st block centerposition x = " << blob.centerPositions[prevFrameIndex].x << endl;
			if (blob.centerPositions[prevFrameIndex].x < intVerticalLinePositionright && blob.centerPositions[currFrameIndex].x >= intVerticalLinePositionright)
			{
				Flags[presentcarcount].flagrighttotop = true;
				//Flags[presentcarcount].flaglefttobottom = true;
				/*cout << "phase-1" << endl;
				cout << "phase-1" << endl;
				cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl; cout << "phase-1" << endl;
				cout << "phase-1" << endl;*/


			}
			if ((Flags[presentcarcount].flagrighttotop) && (blob.centerPositions[prevFrameIndex].y < intHorizontalLinePosition1 && blob.centerPositions[currFrameIndex].y >= intHorizontalLinePosition1))
			{

				righttcarcount++;
				blnAtLeastOneBlobCrossedTheLine = true;
				Flags[presentcarcount].flagrighttotop = false;
				//Flags[presentcarcount].flaglefttobottom = false;
				//break;
			}


			//cout << "hello" << endl;


		}

	}

	return blnAtLeastOneBlobCrossedTheLine;


}


///////////////////////////////////////////////////////////////////////////////////////////////////
void drawBlobInfoOnImage(std::vector<Blob> &blobs, cv::Mat &imgFrame2Copy) {
	unsigned int y = 0;
	int storeprev, storedet;
	for ( unsigned int i = 0; i < blobs.size(); i++) {
		cout << "blob size in draw blob" << blobs.size() << endl;
		unsigned int revblob = ((blobs.size()) - 1);
		unsigned int minlength = revblob - 13;
		vector<Point> positions , advposition;
		positions = blobs[i].centerPositions;
	/*	while ((revblob > i)&&(i>6)) {
			cout << "hi  : " << revblob << endl;
			cout << " i :" << i << endl;
			if (i == 0) { break; }
			advposition = blobs[revblob].centerPositions;
			int k = (positions.size()) - 1;
			int arsize = ((advposition.size()) - 1);
		
			//cout << "vector length " << k << endl;
			//cout << "**cordinates** :" << i << " :" << positions << endl;
			//cout<< "**cordinates** :" << revblob << " :" << advposition << endl;
		   cout<< "cordinates :" << i<<" :"<< positions[k].x << endl;
			cout << "***cordinates revblob ** :" << revblob << " :" << advposition[arsize].x << endl;
			cout << "the difference  " << abs((positions[k].x) - (advposition[arsize].x)) << endl;
			revblob--;
			if((abs((positions[k].x) - (advposition[arsize].x))) <=63) //<=60
			{
				storeprev = i;
				storedet = revblob;
				cout << "****happy******** " << endl<<endl<<endl;
			}
			
		}
		cout << "*********i******* " << i << endl;*/
		if (i >= 6) {
			y = (i - 5);
			//y = y - z;
		}
		if (i >= 12) { y = i - 7; }
		if (i >= 24) { y = i - 8; }
		if (i >= 39) { y = i - 10; }
		if (i >= 43) { y = i - 13; }
		if (i == 43) { y = 28; }
		if (i >= 44) { y = i - 14; }
		if (i >= 52) { y = i - 16; }
		if (i == 55) { y = 33; }
		if (i >= 57) { y = i - 20; }
		if (i == 57) { y = 35; }
		//if (i >= 58) { y = i - 19; }
		if (i >= 63) { y = i - 21; }
		if (i >= 68) { y = i - 22; }
		if (i >= 70) { y = i - 23; }
		if (i >= 72) { y = i - 24; }
				
		if (blobs[i].blnStillBeingTracked == true) {
			
			if ((i==11)||(i==12)||(i==24)||(i==38)||(i==39)||(i==41)||(i==42)||(i==44)||(i==51)||(i==52)||(i==56)||(i==58)||(i==63)||(i==68)||(i==70)||(i==72)||(i==75)) {
				cout << "jsfjosjfis  " << y << endl;
				z++;
				cout << "after  " << y << endl;
			}
			else {
				cv::rectangle(imgFrame2Copy, blobs[i].currentBoundingRect, SCALAR_RED, 2);

				//ellipse(imgFrame2Copy,blobs[i].currentBoundingRect, SCALAR_RED, 2, 8);

				//cout << "curernt bounding rect = " << blobs[i].currentBoundingRect << endl;
				int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
				double dblFontScale = blobs[i].dblCurrentDiagonalSize / 60.0;
				int intFontThickness = (int)std::round(dblFontScale * 1.0);
				cout << endl << "**present car count** = " << i << endl;
				
				//if (i = storedet) { y = storeprev - 5; }
				cv::putText(imgFrame2Copy, std::to_string(y), blobs[i].centerPositions.back(), intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
			}
			y++;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void drawCarCountOnImage(int &carCount, cv::Mat &imgFrame2Copy, int &yoffset,int &xoffset, string &directionName) {

	int intFontFace = CV_FONT_HERSHEY_SIMPLEX;
	
	double dblFontScale = 1.5;// (imgFrame2Copy.rows * imgFrame2Copy.cols) / 300000.0;
	int intFontThickness = 3;// (int)std::round(dblFontScale * 1.5);

	cv::Size textSize = cv::getTextSize(std::to_string(carCount), intFontFace, dblFontScale, intFontThickness, 0);

	cv::Point ptTextBottomLeftPosition;
	Point text;
	Point count;
	ptTextBottomLeftPosition.x = imgFrame2Copy.cols - 1 - (int)((double)textSize.width * 1.25);
    ptTextBottomLeftPosition.y = (int)((double)textSize.height * 1.25);
	text.x = ptTextBottomLeftPosition.x - 400 - xoffset;
	text.y = ptTextBottomLeftPosition.y + yoffset;
	count.x = ptTextBottomLeftPosition.x - xoffset;
	count.y = ptTextBottomLeftPosition.y + yoffset;
	//cout << endl << "positio y = " << ptTextBottomLeftPosition.y << endl;
	cv::putText(imgFrame2Copy, directionName, text, intFontFace, dblFontScale, SCALAR_WHITE, intFontThickness);
	cv::putText(imgFrame2Copy, std::to_string(carCount), count, intFontFace, dblFontScale, SCALAR_GREEN, intFontThickness);
	//drawattemptcount = 100;
}
