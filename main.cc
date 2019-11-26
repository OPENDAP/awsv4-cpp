#include <iostream>
#include "awsv4.h"

// From Amazon (https://docs.aws.amazon.com/general/latest/gr/signature-v4-examples.html)
// The following input

// secret_key = 'wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY'
// dateStamp = '20120215'
// regionName = 'us-east-1'
// serviceName = 'iam'
//
// Should produce this signature (and the interim values leading to it)
// kSecret  = '41575334774a616c725855746e46454d492f4b374d44454e472b62507852666943594558414d504c454b4559'
// kDate    = '969fbb94feb542b71ede6f87fe4d5fa29c789342b0f407474670f0c2489e0a0d'
// kRegion  = '69daa0209cd9c5ff5c8ced464a696fd4252e981430b10e3d3fd8e2f197d7a70c'
// kService = 'f72cfd46f26bc4643f06a11eabb6c0ba18780c19a8da0c31ace671265e3c87fa'
// Signing = 'f4780e2d9f65fa895f9c67b32ce1baf0b0d8a43505a000a1a9e090d414db404d'
//
// jhrg 11/24/19


int main() {

    // This hardcoded date/time value matches the date used in the above AWS
    // example. This code returns the same value for 'Signing' and also produces
    // a 'String to be signed' that looks correct. jhrg 11/24/19
    // 20120215T000000Z
    struct std::tm t;
    t.tm_sec = 0;
    t.tm_min = 00;
    t.tm_hour = 10;
    t.tm_mon = 1;
    t.tm_year = 2012 - 1900;
    t.tm_isdst = -1; 
    t.tm_mday = 15;
    const std::time_t request_date = std::mktime(&t);

    try {
        const std::string auth_header = AWSV4::compute_awsv4_signature("http://iam.amazonaws.com/", request_date,
                "AKIAIOSFODNN7EXAMPLE", "wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY",
                "us-east-1", "iam", true);

        std::cout << std::endl;
        std::cout << "AWS V4 Authorization and date headers:" << std::endl;
        std::cout << "Authorization:" << auth_header << std::endl;
        std::cout << "x-amz-date:" << AWSV4::ISO8601_date(request_date) << std::endl;
    }
    catch (std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
