//#include <iostream>
//#include <sstream>

//#include <openssl/md5.h>
//
//string build_digest(string user, string pwd, string realm, string method,  string uri, string nonce)
//{
//    ostringstream s1, s2;
//
//    s1 << user << ":" << realm << ":" << pwd;
//    s2 << method << ":" << uri;
//
//    MD5_CTX md5handler;
//    unsigned char md5digest[MD5_DIGEST_LENGTH];
//
//    MD5(in, 10, md5digest);
////    h2 = md5("$method:$uri");
////
////    return md5("$h1:$nonce:$h2");
//
//    return "";
//}

//int main(int argc, char** argv) {
//
////    cv::VideoCapture * stream = new cv::VideoCapture("rtsp://192.168.31.197:554/onvif1");
//
//    TCPClient c;
//    const string host = "192.168.31.197";
//
//    c.conn(host , 554);
//
//    //send some data
//    ostringstream data;
//
//    cout << "----------------------------\n\n";
//
//    data.clear();
//    data << "OPTIONS 192.168.31.197:554 RTSP/1.0\r\n";
//    data << "CSeq: 1" << "\r\n";
//    data << "User-Agent: custom" << "\r\n";
//    data << "\r\n";
//
//    c.send_data(data.str());
//    cout << c.receive(1024);
//
//    cout << "\n\n----------------------------\n\n";
//
//    cout << "----------------------------\n\n";
//
//    data.str(std::string());
//    data << "DESCRIBE 192.168.31.197:554 RTSP/1.0\r\n";
//    data << "CSeq: 2" << "\r\n";
//    data << "User-Agent: custom" << "\r\n";
//    data << "Accept: application/sdp" << "\r\n";
//    data << "\r\n";
//
//    c.send_data(data.str());
//    cout << c.receive(1024);
//
//    cout << "\n\n----------------------------\n\n";
//
//    return 0;
//
//}

#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <iostream>

int main(int, char**)
{
    cv::VideoCapture vcap;
    cv::Mat image;    // This works on a D-Link CDS-932L

    //open the video stream and make sure it's opened
    if(!vcap.open("rtsp://127.0.0.1:8000/onvif1"))
    {
        std::cout << "Error opening video stream or file" << std::endl;
        return -1;
    }

    for(;;)
    {
        if(!vcap.read(image))
        {
            std::cout << "No frame" << std::endl;
            cv::waitKey();
        }

        cv::imshow("Output Window", image);
        if(cv::waitKey(1) >= 0)
            break;
    }

}