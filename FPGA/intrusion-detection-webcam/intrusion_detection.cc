#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>

#include "server.h"

char mqtt_cmd[] = "mosquitto_pub -d -h 192.168.0.11 -t /sensor/data -m '{\"serialNumber\":\"SN-0002\",\"intrusion_count_v\": 1}'";
int data_send = 0;

cv::Mat MoveDetect(cv::Mat temp, cv::Mat frame)
{
	cv::Mat result = frame.clone();
	cv::resize(result, result, cv::Size(640, 480));

	//1.将background和frame转为灰度图
	cv::Mat gray1, gray2;
	cv::cvtColor(temp, gray1, CV_BGR2GRAY);
	cv::cvtColor(frame, gray2, CV_BGR2GRAY);

	//2.将background和frame做差
	cv::Mat diff;
	cv::absdiff(gray1, gray2, diff);
	//cv::imshow("diff", diff);

	//3.对差值图diff_thresh进行阈值化处理
	cv::Mat diff_thresh;
	cv::threshold(diff, diff_thresh, 50, 255, CV_THRESH_BINARY);
	//cv::imshow("diff_thresh", diff_thresh);

	//4.腐蚀
	cv::Mat kernel_erode =
	    getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
	cv::Mat kernel_dilate =
	    getStructuringElement(cv::MORPH_RECT, cv::Size(18, 18));
	cv::erode(diff_thresh, diff_thresh, kernel_erode);
	//cv::imshow("erode", diff_thresh);

	//5.膨胀
	cv::dilate(diff_thresh, diff_thresh, kernel_dilate);
	//cv::imshow("dilate", diff_thresh);

	//6.查找轮廓并绘制轮廓
	std::vector < std::vector < cv::Point > >contours;
	cv::findContours(diff_thresh, contours, CV_RETR_EXTERNAL,
			 CV_CHAIN_APPROX_NONE);
	cv::drawContours(result, contours, -1, cv::Scalar(0, 0, 255), 2);

	//在result上绘制轮廓
	//7.查找正外接矩形
	std::vector < cv::Rect > boundRect(contours.size());
	for (unsigned i = 0; i < contours.size(); i++) {
		boundRect[i] = boundingRect(contours[i]);
		rectangle(result, boundRect[i], cv::Scalar(0, 255, 0), 2);
	}

	if (0 < contours.size()) {
        std::cout << "mqtt send success!" << std::endl;
        system(mqtt_cmd);
    }
	return result;
}

int main()
{

    uint16_t port = 5555;                   /// Set the port
    http::Server s(port);

    cv::VideoCapture cap;
	if (!cap.open(0)) {
		std::cerr << "Cannot open the camera!" << std::endl;
		return -1;
	}

    std::mutex mtx;

    s.get("/img", [&] (auto, auto res) {

        res.headers.push_back("Connection: close");
        res.headers.push_back("Max-Age: 0");
        res.headers.push_back("Expires: 0");
        res.headers.push_back("Cache-Control: no-cache, private");
        res.headers.push_back("Pragma: no-cache");
        res.headers.push_back("Content-Type: multipart/x-mixed-replace;boundary=--boundary");

        if (!res.send_header())
            return;

        std::vector<uchar> buf;

        cv::Mat frame;
        cv::Mat temp;
        cv::Mat result;
        int i = 0;        
        while(true) {
            mtx.lock();
            cap >> frame;
            if (frame.empty()) {
                std::cout << "frame is empty!" << std::endl;
                break;
            }
            if (i == 0) {
                result = MoveDetect(frame, frame);
            } else {
                result = MoveDetect(temp, frame);
            }
            mtx.unlock();
            temp = frame.clone();
            i++;

            cv::imencode(".jpg", result, buf);
            std::string image (buf.begin(), buf.end());
            if (!res.send_msg("--boundary\r\n"
                              "Content-Type: image/jpeg\r\n"
                              "Content-Length: " +
                              std::to_string(image.size()) +
                              "\r\n\r\n" +
                              image))
                return;
        }
        cap.release();
	    cv::destroyAllWindows();

    }).get("/", [](auto, auto res) {
        res >> "<html>"
               "    <body>"
               "        <h1>CAMERA STREAMING</h1>"
                /// Set the correct ip address
               "        <img src='http://192.168.0.12:5555/img'/>"
               "    </body>"
               "</html>";
    }).listen();

    return 0;
}
