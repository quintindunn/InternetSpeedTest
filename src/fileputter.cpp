#include <iostream>
#include <vector>
#include <random>

#include "fileputter.h"
#include "requester.h"

FilePutter::FilePutter(const std::string& url, const time_point<high_resolution_clock>& start, size_t size)
    : url(url), starttime(start), donePromise(), doneFuture(donePromise.get_future()), size(size) {}


std::vector<unsigned char> generate_random_bytes(size_t size) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    std::vector<unsigned char> data(size);
    for (size_t i = 0; i < size; ++i) {
        data[i] = static_cast<unsigned char>(dis(gen));
    }

    return data;
}


void execute(FilePutter* ctx, std::promise<void>* promise) {
    using namespace std::chrono;

    try {
        if (duration_cast<seconds>(high_resolution_clock::now() - ctx->getStartTime()).count() > PUTTER_TIMEOUT) {
            ctx->setSize(0);
            promise->set_value();
            return;
        }
        std::vector<unsigned char> data = generate_random_bytes(ctx->getSize());

        post(ctx->getUrl(), data);
        promise->set_value();

    }
    catch (...) {
        ctx->setSize(0);
        promise->set_exception(std::current_exception());
    }
}


void FilePutter::run() {
    try {
        std::thread t(execute, this, &donePromise);
        t.detach();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in FilePutter::run: " << e.what() << std::endl;
    }
}