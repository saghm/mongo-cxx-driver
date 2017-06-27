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

#include <chrono>

#include <mongocxx/instance.hpp>
#include "bson/bson_encoding.hpp"
#include "microbench.hpp"
#include "multi_doc/bulk_insert.hpp"
#include "multi_doc/find_many.hpp"
#include "multi_doc/gridfs_download.hpp"
#include "multi_doc/gridfs_upload.hpp"
#include "single_doc/find_one_by_id.hpp"
#include "single_doc/insert_one.hpp"
#include "single_doc/run_command.hpp"

namespace benchmark {
const std::chrono::duration<int, std::milli> mintime{60000};
const std::chrono::duration<int, std::milli> maxtime{300000};

bool finished_running(const std::chrono::duration<int, std::milli>& curr_time, std::uint32_t iter) {
    return (curr_time > maxtime || (curr_time > mintime && iter > 100));
}

void run_microbench(microbench* bench,
                    bsoncxx::stdx::string_view input_json_filename = bsoncxx::stdx::string_view()) {
    bench->setup(input_json_filename);

    for (std::uint32_t iteration = 0; !finished_running(bench->get_execution_time(), iteration);
         iteration++) {
        bench->before_task();

        bench->do_task();

        bench->after_task();
    }
    bench->teardown();
}

// this would run the benchmarks and collect the data.
int main() {
    mongocxx::instance instance{};

    bson_encoding flat_bson_encode;
    run_microbench(&flat_bson_encode, "FLAT_BSON.json");
    bson_encoding deep_bson_encode;
    run_microbench(&deep_bson_encode, "DEEP_BSON.json");
    bson_encoding full_bson_encode;
    run_microbench(&full_bson_encode, "FULL_BSON.json");
    // TODO CXX-1241: Add bson_decoding equivalents.

    run_command run_command_bench;
    run_microbench(&run_command_bench);
    find_one_by_id find_one_by_id_bench;
    run_microbench(&find_one_by_id_bench, "TWEET.json");
    insert_one small_doc_insert_one(10000);
    run_microbench(&small_doc_insert_one, "SMALL_DOC.json");
    insert_one large_doc_insert_one(10);
    run_microbench(&large_doc_insert_one, "LARGE_DOC.json");

    find_many find_many_bench;
    run_microbench(&find_many_bench, "TWEET.json");
    bulk_insert small_doc_bulk_insert{10000};
    run_microbench(&small_doc_bulk_insert, "SMALL_DOC.json");
    bulk_insert large_doc_bulk_insert{10};
    run_microbench(&large_doc_bulk_insert, "LARGE_DOC.json");
    gridfs_upload gridfs_upload_bench;
    run_microbench(&gridfs_upload_bench, "GRIDFS_LARGE.bin");
    gridfs_download gridfs_download_bench;
    run_microbench(&gridfs_download_bench, "GRIDFS_LARGE.bin");

    // get results from the microbenches...
}
}