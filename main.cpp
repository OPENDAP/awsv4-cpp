#include <iostream>
#include "awsv4.hpp"

// From Amazon (https://docs.aws.amazon.com/general/latest/gr/signature-v4-examples.html)
// The following input

// key = 'wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY'
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

/**
 * @brief Return the 
 * @return
 */
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

    const std::string base_uri{"http://iam.amazonaws.com/"};
    const std::string query_args{""};
    const std::string uri_str{base_uri + "?" + query_args};

    Poco::URI uri;
    try {
        uri = Poco::URI(uri_str);
    } catch (std::exception& e) {
        throw std::runtime_error(e.what());
    }
    uri.normalize();
    const auto canonical_uri = AWSV4::canonicalize_uri(uri);
    
    const auto canonical_query = AWSV4::canonicalize_query(uri);

    const std::string sha256_empty_payload = {"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"};
    // All AWS V4 signature require x-amz-content-sha256. jhrg 11/24/19
    std::vector<std::string> headers{"host: iam.amazonaws.com", "x-amz-date: ", "x-amz-content-sha256: "};
    headers[1].append(AWSV4::ISO8601_date(request_date));
    headers[2].append(sha256_empty_payload);

    const auto canonical_headers_map = AWSV4::canonicalize_headers(headers);
    if (canonical_headers_map.empty()) {
        std::cerr << "headers malformed" << std::endl;
        std::exit(1);
    }
    const auto headers_string = AWSV4::map_headers_string(canonical_headers_map);
    const auto signed_headers = AWSV4::map_signed_headers(canonical_headers_map);

    // const std::string payload{""};
    //
    // For GET requests, the payload is always the empty string, which has a known hash. jhrg 11/24/19
    // unused const std::string sha256_payload = {"e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855"};
    
    const auto canonical_request = AWSV4::canonicalize_request(AWSV4::GET,
                                                               canonical_uri,
                                                               canonical_query,
                                                               headers_string,
                                                               signed_headers,
                                                               sha256_empty_payload);
    
    std::cout << "--\n" << canonical_request << "\n--\n" << std::endl;

    auto hashed_canonical_request = AWSV4::sha256_base16(canonical_request); 
    std::cout << hashed_canonical_request << std::endl;

    const std::string region{"us-east-1"};
    const std::string service{"iam"};

    auto credential_scope = AWSV4::credential_scope(request_date,region,service);

    auto string_to_sign = AWSV4::string_to_sign(AWSV4::STRING_TO_SIGN_ALGO,
                                                request_date,
                                                credential_scope,
                                                hashed_canonical_request);

    std::cout << "--\n" << string_to_sign << "\n----\n" << std::endl;

    // const std::string date_stamp = "20120215";
    const std::string key = "wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY";

    auto signature = AWSV4::calculate_signature(request_date,
                                                key,
                                                region,
                                                service,
                                                string_to_sign);
    
    std::cout << signature << std::endl;
    return 0;
}
