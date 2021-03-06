// Copyright 2016 MongoDB Inc.
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

#include "helpers.hpp"

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/test_util/catch.hh>
#include <mongocxx/instance.hpp>
#include <mongocxx/options/update.hpp>

namespace {
using namespace bsoncxx::builder::stream;
using namespace mongocxx;

TEST_CASE("update opts", "[update][option]") {
    instance::current();

    options::update updt;

    auto collation = document{} << "locale"
                                << "en_US" << finalize;

    CHECK_OPTIONAL_ARGUMENT(updt, bypass_document_validation, true);
    CHECK_OPTIONAL_ARGUMENT(updt, collation, collation.view());
    CHECK_OPTIONAL_ARGUMENT(updt, upsert, true);
    CHECK_OPTIONAL_ARGUMENT(updt, write_concern, write_concern{});
}

TEST_CASE("update options equals", "[update][option]") {
    instance::current();

    options::update update1{};
    options::update update2{};

    REQUIRE(update1 == update2);
}

TEST_CASE("update options inequals", "[update][option]") {
    instance::current();

    options::update update1{};
    update1.upsert(false);
    options::update update2{};

    REQUIRE(update1 != update2);
}

}  // namespace
