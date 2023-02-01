#include "config/json_archive.hpp"
#include "misc/source.hpp"

#include <fstream>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>

using namespace testing;
using namespace misc;
using namespace config;

namespace
{

class json_archive_tests : public Test
{
protected:
    void SetUp() { archive = std::make_unique< json_archive >(); }

    std::unique_ptr< json_archive > archive;
};

TEST_F(
    json_archive_tests,
    importing_an_non_json_file_path_throws_invalid_file_path)
{
    constexpr auto rel_path = "./json_archive_tests.cpp";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    EXPECT_THROW(import(*archive, abs_path), invalid_file_path);
}

TEST_F(
    json_archive_tests,
    importing_an_non_existent_json_file_path_throws_invalid_file_path)
{
    constexpr auto rel_path = "./blah.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_FALSE(std::filesystem::exists(abs_path));

    EXPECT_THROW(import(*archive, abs_path), invalid_file_path);
}

TEST_F(
    json_archive_tests,
    when_importing_an_existent_json_file_then_its_archived_with_its_path)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    import(*archive, abs_path);
    EXPECT_TRUE(archive->archived(abs_path.c_str()));
}

TEST_F(
    json_archive_tests,
    when_importing_an_existent_json_file_twice_then_its_archived_with_its_path)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    import(*archive, abs_path);
    import(*archive, abs_path);
    EXPECT_TRUE(archive->archived(abs_path.c_str()));
}

TEST_F(json_archive_tests, a_non_imported_json_file_is_not_archived)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    EXPECT_FALSE(archive->archived(abs_path.c_str()));
}

TEST_F(
    json_archive_tests,
    when_accessing_a_non_imported_json_file_the_application_aborts)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    ASSERT_DEATH(archive->get(abs_path.c_str()), "");
}

TEST_F(
    json_archive_tests,
    when_accessing_an_imported_json_file_the_application_doesnt_abort)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    import(*archive, abs_path);
    ASSERT_TRUE(archive->archived(abs_path.c_str()));
    ASSERT_NO_FATAL_FAILURE(archive->get(abs_path.c_str()));
}

TEST_F(
    json_archive_tests,
    when_accessing_an_imported_json_file_the_imported_one_is_returned)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    auto json = import(abs_path);
    import(*archive, abs_path);

    ASSERT_TRUE(archive->archived(abs_path.c_str()));
    EXPECT_EQ(archive->get(abs_path.c_str()), json);
}

TEST_F(
    json_archive_tests,
    dumping_to_a_non_existent_file_path_throws_an_invalid_file_path_error)
{
    constexpr auto rel_path = "blah.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_FALSE(std::filesystem::exists(abs_path));

    json_t json;

    EXPECT_THROW(dump(json, rel_path), invalid_file_path);
}

TEST_F(
    json_archive_tests,
    dumping_to_an_existent_non_json_file_path_throws_an_invalid_file_path_error)
{
    constexpr auto rel_path = "json_archive_tests.cpp";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    json_t json;

    EXPECT_THROW(dump(json, rel_path), invalid_file_path);
}

TEST_F(json_archive_tests, dumping_to_an_existent_json_file_path_doesnt_throw)
{
    constexpr auto rel_path = "../../data/testfiles/sample.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    json_t json;

    EXPECT_NO_THROW(dump(json, rel_path));
}

TEST_F(json_archive_tests, when_importing_dumped_json_file_the_jsons_match)
{
    constexpr auto rel_path = "imported.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_FALSE(std::filesystem::exists(abs_path));
    std::ofstream outfile(abs_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    json_t json;
    json.append("Type");
    dump(json, abs_path);

    EXPECT_EQ(import(abs_path), json);

    std::filesystem::remove(abs_path);
}

TEST_F(json_archive_tests, dumped_jsons_from_the_archive_can_be_reimported)
{
    constexpr auto rel_path = "imported.json";
    const auto abs_path = resolve_source_path(rel_path);
    ASSERT_FALSE(std::filesystem::exists(abs_path));
    std::ofstream outfile(abs_path);
    ASSERT_TRUE(std::filesystem::exists(abs_path));

    json_t json;
    json.append("Type");
    archive->put(abs_path, std::move(json));
    dump(*archive, abs_path);

    EXPECT_EQ(archive->get(abs_path.c_str()), import(abs_path));

    std::filesystem::remove(abs_path);
}

} // namespace