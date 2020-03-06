#include <core/http.h>
#include <core/sdl.h>


class client_app
{
private:
    std::shared_ptr<ice::core::http_client> _http_client;
    std::shared_ptr<ice::core::sdl_app> _sdl_app;
    std::shared_ptr<ice::core::http_headers> _base_headers;
    ice::core::keyboard_callback_t _on_key;
    ice::core::quit_callback_t _on_quit;
    std::atomic_bool _loop;
public:
    
    client_app(
        const std::shared_ptr<ice::core::engine>& engine)
    {
        _http_client = std::make_shared<ice::core::http_client>(
            engine,
            ice::core::string_type("127.0.0.1"),
            3000);
        
        _sdl_app = std::make_shared<ice::core::sdl_app>(
            engine);
        
        _loop.store(true);

        _on_key = [this](const ice::core::key_event& event)
        {
            auto result = this->do_post(
                "/keyEvent",
                "key event");
            if(!result.first)
            {
                this->_loop.store(false);
                throw std::runtime_error(
                    "Post failed");
            }
            else
            {
                std::cerr << result.second 
                          << std::endl;
            }
        };

        _on_quit = [this]()
        {
            this->_loop.store(false);
        };
    }

    ~client_app() = default;

    std::pair<bool,ice::core::string_type> do_get(
        const ice::core::string_type &path) noexcept
    {
        std::pair<bool,ice::core::string_type> result;
        ice::core::http_headers headers;
        
        headers["Content-Type"] = "text/html";
        result.first = true;
        
        try
        {
            result.second = _http_client->get_request(
                path,
                headers);
        }
        catch(const std::exception& e)
        {
            result.first = false;
            result.second = std::string(
                e.what());
        }

        return result;
    }

    std::pair<bool,ice::core::string_type> do_post(
        const ice::core::string_type &path,  
        const std::string & text_payload) noexcept
    {
        std::pair<bool,ice::core::string_type> result;
        ice::core::http_headers headers;
        
        headers["Content-Type"] = "text/html";
        result.first = true;
        
        try
        {
            result.second = _http_client->post_request(
                path,
                headers,
                (ice::core::byte_type*)text_payload.data(),
                text_payload.size());
        }
        catch(const std::exception& e)
        {
            result.first = false;
            result.second = std::string(
                e.what());
        }

        return result;
    }

    std::shared_ptr<ice::core::sdl_window> create_window(
        const ice::core::size_type width,
        const ice::core::size_type height)
    {
        return _sdl_app->create_window(
            "Sample HTTP Client",
            width,
            height);
    }

    void main_loop()
    {
        while(_loop.load())
        {
            _sdl_app->sdl_main(
                _on_key,
                _on_quit,
                std::cerr);
            std::this_thread::sleep_for(
                std::chrono::duration<double,std::milli>(500));
        }
    }
};


int main(const int argc, const char ** argv)
{
    std::shared_ptr<ice::core::engine> engine;
    
    std::shared_ptr<client_app> client;

    std::shared_ptr<ice::core::sdl_window> window;

    engine = std::make_shared<ice::core::engine>(4);
    
    client = std::make_shared<client_app>(engine);

    window = client->create_window(
        640,
        480);

    client->main_loop();

    return 0;
}