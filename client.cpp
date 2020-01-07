#include <uWS/uWS.h>
#include <thread>
#include <algorithm>
#include <iostream>

int main() {
    std::vector<std::unique_ptr<std::thread>> threads(std::thread::hardware_concurrency());
    std::transform(threads.begin(), threads.end(), threads.begin(), [](std::unique_ptr<std::thread> t) {
        return new std::thread([]() {
            uWS::Hub h;
            h.connect("ws://localhost:3000",nullptr);
            h.run();
        });
    });

    std::for_each(threads.begin(), threads.end(), [](std::unique_ptr<std::thread> t) {
        t->join();
    });
    return 0;
}
