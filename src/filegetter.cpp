#include <string>
#include <curl/curl.h>
#include <chrono>

#include <thread>
#include <future>

#include <iostream>

#include "filegetter.h"
#include "requester.h"


FileGetter::FileGetter(const std::string& url, const time_point<high_resolution_clock>& start)
    : url(url), starttime(start), donePromise(), doneFuture(donePromise.get_future()) {}


void execute(FileGetter* ctx, std::promise<void>* promise) {
    using namespace std::chrono;

    try {
        if (duration_cast<seconds>(high_resolution_clock::now() - ctx->getStartTime()).count() > GETTER_TIMEOUT) {
            std::string result = "";
            ctx->setResult(result);
        }
        else {
            std::string result = get(ctx->getUrl());
            ctx->setResult(result);
        }
        promise->set_value();
    }
    catch (...) {
        promise->set_exception(std::current_exception());
    }
}

void FileGetter::run() {
    try {
        std::thread t(execute, this, &donePromise);
        t.detach();
     }
    catch (const std::exception& e) {
        std::cerr << "Exception in FileGetter::run: " << e.what() << std::endl;
    }
    printf("%s", getResult().c_str());

}