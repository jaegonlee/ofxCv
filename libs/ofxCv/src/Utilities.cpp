#include "ofxCv/Utilities.h"

#include "ofGraphicsBaseTypes.h"
#include "ofMath.h"
#include "ofMesh.h"


// vs2010 support (this should be added to the OF core)
#if (_MSC_VER)
#include <stdint.h>
#endif

namespace ofxCv {
	
	using namespace cv;
	using namespace std;

	Mat toCv(Mat& mat) {
		return mat;
	}

    Mat toCv(const Mat& mat) {
        return mat.clone();
    }

	Point2f toCv(glm::vec2 vec) {
		return Point2f(vec.x, vec.y);
	}
	
	Point3f toCv(glm::vec3 vec) {
		return Point3f(vec.x, vec.y, vec.z);
	}
	
	cv::Rect toCv(ofRectangle rect) {
		return cv::Rect(rect.x, rect.y, rect.width, rect.height);
	}
	
	Mat toCv(ofMesh& mesh) {
		std::vector<glm::vec3>& vertices = mesh.getVertices();
		return Mat(1, vertices.size(), CV_32FC3, &vertices[0]);
	}
	
    std::vector<cv::Point2f> toCv(const ofPolyline& polyline) {
		// if polyline.getVertices() were const, this could wrap toCv(vec<vec2f>)
		std::vector<cv::Point2f> contour(polyline.size());
		for(int i = 0; i < polyline.size(); i++) {
			contour[i].x = polyline[i].x;
			contour[i].y = polyline[i].y;
		}
		return contour;		
	}
	
	std::vector<cv::Point2f> toCv(const std::vector<glm::vec2>& points) {
		std::vector<cv::Point2f> out(points.size());
		for(int i = 0; i < points.size(); i++) {
			out[i].x = points[i].x;
			out[i].y = points[i].y;
		}
		return out;		
	}
	
	std::vector<cv::Point3f> toCv(const std::vector<glm::vec3>& points) {
		std::vector<cv::Point3f> out(points.size());
		for(int i = 0; i < points.size(); i++) {
			out[i].x = points[i].x;
			out[i].y = points[i].y;
			out[i].z = points[i].z;
		}
		return out;		
	}
	
	Scalar toCv(ofColor color) {
		return Scalar(color.r, color.g, color.b, color.a);
	}
	
	glm::vec2 toOf(Point2f point) {
		return glm::vec2(point.x, point.y);
	}
	
	glm::vec3 toOf(Point3f point) {
		return glm::vec3(point.x, point.y, point.z);
	}
	
	ofRectangle toOf(cv::Rect rect) {
		return ofRectangle(rect.x, rect.y, rect.width, rect.height);
	}
	
	ofPolyline toOf(cv::RotatedRect rect) {
		std::vector<cv::Point2f> corners(4);
		rect.points(&corners[0]);
		ofPolyline polyline = toOf(corners);
		return polyline;
	}
	
	float getMaxVal(int cvDepth) {
		switch(cvDepth) {
			case CV_8U: return std::numeric_limits<uint8_t>::max();
			case CV_16U: return std::numeric_limits<uint16_t>::max();
				
			case CV_8S: return std::numeric_limits<int8_t>::max();
			case CV_16S: return std::numeric_limits<int16_t>::max();
			case CV_32S: return std::numeric_limits<int32_t>::max();
				
			case CV_32F: return 1;
			case CV_64F: default: return 1;
		}
	}
	
	float getMaxVal(const Mat& mat) {
		return getMaxVal(mat.depth());
	}
	
	// for some reason, cvtColor handles this info internally rather than having
	// a single helper function. so we have to create a helper function to aid
	// in doing the allocationg ofxCv::convertColor()
#define mkcase(x, y) {case x: return y;}
	int getTargetChannelsFromCode(int conversionCode) {
		switch(conversionCode) {
                mkcase(COLOR_RGB2RGBA,4)	mkcase(COLOR_RGBA2RGB,3) mkcase(COLOR_RGB2BGRA,4)
                mkcase(COLOR_RGBA2BGR,3) mkcase(COLOR_BGR2RGB,3) mkcase(COLOR_BGRA2RGBA,4)
                mkcase(COLOR_BGR2GRAY,1) mkcase(COLOR_RGB2GRAY,1) mkcase(COLOR_GRAY2RGB,3)
                mkcase(COLOR_GRAY2RGBA,4) mkcase(COLOR_BGRA2GRAY,1) mkcase(COLOR_RGBA2GRAY,1)
                mkcase(COLOR_BGR5652BGR,3) mkcase(COLOR_BGR5652RGB,3) mkcase(COLOR_BGR5652BGRA,4)
                mkcase(COLOR_BGR5652RGBA,4) mkcase(COLOR_BGR5652GRAY,1) mkcase(COLOR_BGR5552BGR,3)
                mkcase(COLOR_BGR5552RGB,3) mkcase(COLOR_BGR5552BGRA,4) mkcase(COLOR_BGR5552RGBA,4)
				mkcase(COLOR_BGR5552GRAY,1) mkcase(COLOR_BGR2XYZ,3) mkcase(COLOR_RGB2XYZ,3)
				mkcase(COLOR_XYZ2BGR,3) mkcase(COLOR_XYZ2RGB,3) mkcase(COLOR_BGR2YCrCb,3)
				mkcase(COLOR_RGB2YCrCb,3) mkcase(COLOR_YCrCb2BGR,3) mkcase(COLOR_YCrCb2RGB,3)
				mkcase(COLOR_BGR2HSV,3) mkcase(COLOR_RGB2HSV,3) mkcase(COLOR_BGR2Lab,3)
				mkcase(COLOR_RGB2Lab,3) mkcase(COLOR_BayerGB2BGR,3) mkcase(COLOR_BayerBG2RGB,3)
				mkcase(COLOR_BayerGB2RGB,3) mkcase(COLOR_BayerRG2RGB,3) mkcase(COLOR_BGR2Luv,3)
				mkcase(COLOR_RGB2Luv,3) mkcase(COLOR_BGR2HLS,3) mkcase(COLOR_RGB2HLS,3)
				mkcase(COLOR_HSV2BGR,3) mkcase(COLOR_HSV2RGB,3) mkcase(COLOR_Lab2BGR,3)
				mkcase(COLOR_Lab2RGB,3) mkcase(COLOR_Luv2BGR,3) mkcase(COLOR_Luv2RGB,3)
				mkcase(COLOR_HLS2BGR,3) mkcase(COLOR_HLS2RGB,3) mkcase(COLOR_BayerBG2RGB_VNG,3)
				mkcase(COLOR_BayerGB2RGB_VNG,3) mkcase(COLOR_BayerRG2RGB_VNG,3)
				mkcase(COLOR_BayerGR2RGB_VNG,3) mkcase(COLOR_BGR2HSV_FULL,3)
				mkcase(COLOR_RGB2HSV_FULL,3) mkcase(COLOR_BGR2HLS_FULL,3)
				mkcase(COLOR_RGB2HLS_FULL,3) mkcase(COLOR_HSV2BGR_FULL,3)
				mkcase(COLOR_HSV2RGB_FULL,3) mkcase(COLOR_HLS2BGR_FULL,3)
				mkcase(COLOR_HLS2RGB_FULL,3) mkcase(COLOR_LBGR2Lab,3) mkcase(COLOR_LRGB2Lab,3)
				mkcase(COLOR_LBGR2Luv,3) mkcase(COLOR_LRGB2Luv,3) mkcase(COLOR_Lab2LBGR,4)
				mkcase(COLOR_Lab2LRGB,4) mkcase(COLOR_Luv2LBGR,4) mkcase(COLOR_Luv2LRGB,4)
				mkcase(COLOR_BGR2YUV,3) mkcase(COLOR_RGB2YUV,3) mkcase(COLOR_YUV2BGR,3)
				mkcase(COLOR_YUV2RGB,3)
			default: return 0;
		}
	}
}
