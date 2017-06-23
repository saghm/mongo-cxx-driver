// Copyright 2017 MongoDB Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include <bsoncxx/document/value.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/stdx/string_view.hpp>

namespace benchmark {
class microbench {
   public:
    microbench() {}

    virtual void setup(const bsoncxx::stdx::string_view) : _score{0} {}

    virtual void before_task() {}

    void do_task();

    virtual void after_task() {}

    virtual void teardown() {}

    const benchmark_score& get_results() const {
        return _score;
    }

    const std::chrono::duration<int, std::milli>& get_execution_time() const {
        return _score.get_execution_time();
    }

   protected:
    virtual void task() {}

    benchmark_score _score;
}

void microbench::do_task() {
    scores.start_sample();
    task();
    scores.end_sample();
}

std::vector<std::string> parse_json_file_to_strings(bsoncxx::stdx::string_view json_file) {
    std::vector<std::strings> jsons;
    std::ifstream stream{json_file};
    while (!stream.eof()) {
        std::string s;
        std::getline(stream, s);
        jsons->push_back(s);
    }
    return jsons;
}

std::vector<bsoncxx::document::value> parse_json_file_to_documents(
    bsoncxx::stdx::string_view json_file) {
    std::vector<bsoncxx::document::value> docs;
    std::ifstream stream{json_file};
    while (!stream.eof()) {
        std::string s;
        std::getline(stream, s);
        docs->push_back(bsoncxx::from_json(bsoncxx::stdx::string_view{s}));
    }
    return docs;
}

std::vector<std::string> parse_documents_to_bson(
    const std::vector<bsoncxx::document::value>& docs) {
    std::vector<std::string> bsons;
    for (std::uint32_t i = 0; i < docs.size(); i++) {
        bsons->push_back(bsoncxx::to_json(docs[i]));
    }
    return bsons;
}
}
