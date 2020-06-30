#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdlib.h>
#include <unistd.h>

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";

static inline bool is_base64(unsigned char c)
{
	return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const *bytes_to_encode,
			  unsigned int in_len)
{
	std::string ret;
	int i = 0;
	int j = 0;
	unsigned char char_array_3[3];
	unsigned char char_array_4[4];

	while (in_len--) {
		char_array_3[i++] = *(bytes_to_encode++);
		if (i == 3) {
			char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
			char_array_4[1] =
			    ((char_array_3[0] & 0x03) << 4) +
			    ((char_array_3[1] & 0xf0) >> 4);
			char_array_4[2] =
			    ((char_array_3[1] & 0x0f) << 2) +
			    ((char_array_3[2] & 0xc0) >> 6);
			char_array_4[3] = char_array_3[2] & 0x3f;

			for (i = 0; (i < 4); i++)
				ret += base64_chars[char_array_4[i]];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 3; j++)
			char_array_3[j] = '\0';

		char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
		char_array_4[1] =
		    ((char_array_3[0] & 0x03) << 4) +
		    ((char_array_3[1] & 0xf0) >> 4);
		char_array_4[2] =
		    ((char_array_3[1] & 0x0f) << 2) +
		    ((char_array_3[2] & 0xc0) >> 6);

		for (j = 0; (j < i + 1); j++)
			ret += base64_chars[char_array_4[j]];

		while ((i++ < 3))
			ret += '=';

	}

	return ret;

}

std::string base64_decode(std::string const &encoded_string)
{
	size_t in_len = encoded_string.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	std::string ret;

	while (in_len-- && (encoded_string[in_] != '=')
	       && is_base64(encoded_string[in_])) {
		char_array_4[i++] = encoded_string[in_];
		in_++;
		if (i == 4) {
			for (i = 0; i < 4; i++)
				char_array_4[i] =
				    base64_chars.find(char_array_4[i]) & 0xff;

			char_array_3[0] =
			    (char_array_4[0] << 2) +
			    ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] =
			    ((char_array_4[1] & 0xf) << 4) +
			    ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] =
			    ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = 0; j < i; j++)
			char_array_4[j] =
			    base64_chars.find(char_array_4[j]) & 0xff;

		char_array_3[0] =
		    (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] =
		    ((char_array_4[1] & 0xf) << 4) +
		    ((char_array_4[2] & 0x3c) >> 2);

		for (j = 0; (j < i - 1); j++)
			ret += char_array_3[j];
	}

	return ret;
}

char mqtt_cmd[] = "mosquitto_pub -d -h 192.168.0.11 -t /sensor/data -m '{\"serialNumber\":\"SN-0002\",\"intrusion_pic\": \"%s\",\"intrusion_count_v\": 1}'";
int data_send = 0;

cv::Mat MoveDetect(cv::Mat temp, cv::Mat frame)
{
	cv::Mat result = frame.clone();
	cv::resize(result, result, cv::Size(120, 100));

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

	char buffer[1024*1024];
	if (0 < contours.size()) {
		std::vector<uchar> buf;
		cv::imencode(".jpg", result, buf);
		uchar *enc_msg = new uchar[buf.size()];
		for(int i=0; i < buf.size(); i++){
			enc_msg[i] = buf[i];
		}
		std::string encoded = base64_encode(enc_msg, buf.size());
		char *encoded_img = const_cast<char*>(encoded.c_str());
		sprintf(buffer, mqtt_cmd, encoded_img);
#if DEBUG
		std::cout<<buffer<<std::endl;
#endif
		if(data_send % 4 == 0) {
			system(buffer);
			sleep(1);
		}
		data_send++;
	}
	return result;
}

int main()
{
	cv::VideoCapture cap;
	if (!cap.open(0)) {
		std::cerr << "Cannot open the camera!" << std::endl;
		return -1;
	}
	cv::Mat frame;
	cv::Mat temp;
	cv::Mat result;
	int i = 0;
	while (1) {
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
#if DEBUG
		cv::imshow("result", result);
		if (cv::waitKey(10) == 27) {
			std::cout << "exit" << std::endl;
			break;
		}
#endif
		temp = frame.clone();
		i++;
	}
	cap.release();
	cv::destroyAllWindows();
	return 0;
}