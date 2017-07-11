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

#include "../microbench.hpp"

namespace benchmark {

class bson_decoding : public microbench {
   public:
    bson_decoding() = delete;

    bson_decoding(std::string name, double task_size, bsoncxx::stdx::string_view json_file)
        : microbench{name, task_size, std::set<benchmark_type>{benchmark_type::bson_bench}},
          _file_name{json_file.to_string()} {}

   protected:
    void task();
    void setup();

   private:
    std::string _file_name;
    std::string _json;
};

void bson_decoding::setup() {
    _json = parse_json_file_to_strings(_file_name)[0];
}

void bson_decoding::task() {
    for (std::uint32_t i = 0; i < 10000; i++) {
        // TODO CXX-1241: call bson_as_extended json on _json.
    }
}
}