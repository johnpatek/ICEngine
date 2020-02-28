#include <core/http.h>

int main(const int argc, const char ** argv)
{
    std::shared_ptr<ice::core::engine> engine = std::make_shared<
        ice::core::engine>(4);
    
    ice::core::http_headers headers;

    std::string request("This is a request");

    headers["Content-Type"] = "text/html";
    
    ice::core::http_client client(
        engine,
        ice::core::string_type("127.0.0.1"),
        3000);

    std::cerr << client.get_request(
        "/sampleGet",
        headers) << std::endl;

    std::cerr << client.post_request(
        "/samplePost",
        headers,
        (ice::core::byte_type*)request.data(),
        request.size()) << std::endl;
    
    return 0;
}