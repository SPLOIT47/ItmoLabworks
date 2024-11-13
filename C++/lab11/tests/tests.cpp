#include "gtest/gtest.h"
#include "src/DataBase/DataBase.hpp"
#include "src/InputParser.hpp"
#include "src/Indexer.hpp"
#include "src/SimpleSearchEngine.hpp"
#include "src/structures/__lt.hpp"
#include"src/DataBase/DataBase.hpp"
#include <vector>
#include <map>
#include <string>
#include <cstring>

char** convertToCharArray(const std::vector<std::string>& args) {
    char** argv = new char*[args.size() + 1];
    for (size_t i = 0; i < args.size(); ++i) {
        argv[i] = new char[args[i].size() + 1];
        std::strcpy(argv[i], args[i].c_str());
    }
    argv[args.size()] = nullptr;
    return argv;
}

void freeCharArray(char** argv, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        delete[] argv[i];
    }
    delete[] argv;
}

std::vector<std::pair<int64_t, double>> sortMapByValue(const std::map<int64_t, double>& map) {
    std::vector<std::pair<int64_t, double>> vec(map.begin(), map.end());
    std::sort(vec.begin(), vec.end(), [](const std::pair<int64_t, double>& a, const std::pair<int64_t, double>& b) {
        return a.second > b.second;
    });

    return vec;
  }
  
TEST(DataBaseTest, CreateDatabase) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/CreateDatabase");
    ASSERT_TRUE(std::filesystem::exists("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/CreateDatabase/.database"));
}

TEST(DataBaseTest, AddTable) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/AddTable");
    db.AddTable("test_table", 1e5, 10);
    ASSERT_TRUE(std::filesystem::exists("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/AddTable/.database/test_table.txt"));
}

TEST(DataBaseTest, WriteToTable) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/WriteToTable");
    db.AddTable("test_table", 1e5, 10);
    db.Add("test_table", "key1", "value1");
    std::string result = db.get("test_table", "key1");
    ASSERT_EQ(result, "value1");
}

TEST(DataBaseTest, WriteToMultipleTables) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/WriteToMultipleTables");
    db.AddTable("table1", 1e5, 10);
    db.AddTable("table2", 1e5, 10);
    db.Add("table1", "key1", "value1");
    db.Add("table2", "key2", "value2");
    std::string result1 = db.get("table1", "key1");
    std::string result2 = db.get("table2", "key2");
    ASSERT_EQ(result1, "value1");
    ASSERT_EQ(result2, "value2");
}

TEST(DataBaseTest, UpdateValue) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/UpdateValue");
    db.AddTable("test_table", 1e5, 10);
    db.Add("test_table", "key1", "value1");
    db.Add("test_table", "key1", "value2");
    std::string result = db.get("test_table", "key1");
    ASSERT_EQ(result, "value2");
}

TEST(DataBaseTest, HandleCollision) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/HandleCollision");
    db.AddTable("test_table", 1e5, 10);
    db.Add("test_table", "key1", "value1");
    db.Add("test_table", "key2", "value2");
    std::string result1 = db.get("test_table", "key1");
    std::string result2 = db.get("test_table", "key2");
    ASSERT_EQ(result1, "value1");
    ASSERT_EQ(result2, "value2");
}

TEST(DataBaseTest, MultipleCollisions) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/MultipleCollisions");
    db.AddTable("test_table", 1e5, 1);
    db.Add("test_table", "test26", "value1");
    db.Add("test_table", "another29", "value2");
    db.Add("test_table", "test25874", "value3");
    db.Add("test_table", "another2173", "value4");
    std::string result = db.get("test_table", "test26");
    ASSERT_EQ(result, "value1");
    result = db.get("test_table", "another29");
    ASSERT_EQ(result, "value2");
    result = db.get("test_table", "test25874");
    ASSERT_EQ(result, "value3");
    result = db.get("test_table", "another2173");
    ASSERT_EQ(result, "value4");
}

TEST(DataBaseTest, GetNonExistentKey) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/GetNonExistentKey");
    db.AddTable("test_table", 1e5, 10);
    std::string result = db.get("test_table", "nonexistent_key");
    ASSERT_EQ(result, "");
}

TEST(DataBaseTest, CheckDirectoryStructure) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/CheckDirectoryStructure");
    db.AddTable("test_table", 1e5, 10);
    ASSERT_TRUE(std::filesystem::exists("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/CheckDirectoryStructure/.database/test_table.txt"));
    ASSERT_TRUE(std::filesystem::exists("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/CheckDirectoryStructure/.database"));
}

TEST(DataBaseTest, WriteEmptyValue) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/TEST");
    db.AddTable("test_table", 1e5, 10);
    db.Add("test_table", "key1", "");
    std::string result = db.get("test_table", "key1");
    ASSERT_EQ(result, "");
}

TEST(DataBaseTest, WriteLargeValue) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/WriteLargeValueTEST");
    db.AddTable("test_table", 1e5, 10);
    std::string large_value(1000, 'a');
    db.Add("test_table", "key1", large_value);
    std::string result = db.get("test_table", "key1");
    ASSERT_EQ(result, large_value);
}

TEST(DataBaseTest, MultipleTablesWithSameKeys) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/MultipleTablesWithSameKeysTEST");
    db.AddTable("table1", 1e5, 10);
    db.AddTable("table2", 1e5, 10);
    db.Add("table1", "key1", "value1_table1");
    db.Add("table2", "key1", "value1_table2");
    std::string result1 = db.get("table1", "key1");
    std::string result2 = db.get("table2", "key1");
    ASSERT_EQ(result1, "value1_table1");
    ASSERT_EQ(result2, "value1_table2");
}

TEST(DataBaseTest, AddMultipleTables) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/DataBaseTest/AddMultipleTablesTEST");
    db.AddTable("table1", 1e5, 10);
    db.AddTable("table2", 1e5, 20);
    db.Add("table1", "key1", "value1");
    db.Add("table2", "key2", "value2");
    ASSERT_EQ(db.get("table1", "key1"), "value1");
    ASSERT_EQ(db.get("table2", "key2"), "value2");
}

TEST(FileIndexerTest, SimpleFile) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/FileIndexerTest/SimpleFileTEST");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork11-SPLOIT47/tests/texts/aboba.txt"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    freeCharArray(argv, args.size());
    ASSERT_EQ("/Users/alex/labwork11-SPLOIT47/tests/texts/aboba.txt", db.get("files_path", "1"));
}

TEST(FileIndexerTest, MultipleFiles) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/FileIndexerTest/MultipleFilesTEST");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork11-SPLOIT47/tests/texts"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    freeCharArray(argv, args.size());
    ASSERT_EQ("/Users/alex/labwork11-SPLOIT47/tests/texts/aboba.txt", db.get("files_path", "2"));
    ASSERT_EQ("/Users/alex/labwork11-SPLOIT47/tests/texts/test1.txt", db.get("files_path", "0"));
    ASSERT_EQ("/Users/alex/labwork11-SPLOIT47/tests/texts/test2.txt", db.get("files_path", "1"));
}

TEST(FileIndexerTest, MultipleFilesWithCollisions) {
        DataBase db("/Users/alex/labwork11-SPLOIT47/tests/FileIndexerTest/MultipleFilesWithCollisionsTEST");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork3-SPLOIT47"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  500, 1);
    ip.ParseHeader(2, argv, 1);
    freeCharArray(argv, args.size());
    ASSERT_EQ("/Users/alex/labwork3-SPLOIT47/lib/SandModel.h", db.get("files_path", "10"));
    ASSERT_EQ("/Users/alex/labwork3-SPLOIT47/lib/SandModel.cpp", db.get("files_path", "5"));
}

TEST(PostingListGenerationTest, TEST1) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/PostingListGenerationTest/TEST1");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork11-SPLOIT47/tests/texts"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    Indexer id(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    std::string input = "biba";
    tree.clear();
    id.clear();
    ip.ParseCommand(input);
    db.AddTable("posting_lists", ip.longestTerm() + 1 + 2*ip.size(), ip.termsCount());
    id.GeneratePostingLists(ip.size());
    freeCharArray(argv, args.size());
    ASSERT_EQ("1 2", db.get("posting_lists", "biba"));
}

TEST(PostingListGenerationTest, TEST2) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/PostingListGenerationTest/TEST2");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork11-SPLOIT47/tests/texts"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    Indexer id(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    std::string input = "biba OR boba";
    tree.clear();
    id.clear();
    ip.ParseCommand(input);
    db.AddTable("posting_lists", ip.longestTerm() + 1 + 2*ip.size(), ip.termsCount());
    id.GeneratePostingLists(ip.size());
    freeCharArray(argv, args.size());
    ASSERT_EQ("1 2", db.get("posting_lists", "biba"));
    ASSERT_EQ("0 1 2", db.get("posting_lists", "boba"));
}

TEST(PostingListGenerationTest, TEST3) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/PostingListGenerationTest/TEST3");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork3-SPLOIT47"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    Indexer id(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    std::string input = "for";
    tree.clear();
    id.clear();
    ip.ParseCommand(input);
    db.AddTable("posting_lists", ip.longestTerm() + 1 + 2*ip.size(), ip.termsCount());
    id.GeneratePostingLists(ip.size());
    freeCharArray(argv, args.size());
    ASSERT_EQ("", db.get("posting_lists", "biba"));
    ASSERT_EQ("5 9 12 13 16 17 20", db.get("posting_lists", "for"));
}

TEST(ScoreTEST, TEST1) {
        DataBase db("/Users/alex/labwork11-SPLOIT47/tests/ScoreTEST/TEST1");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork11-SPLOIT47/tests/texts"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    Indexer id(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    std::string input = "biba";
    tree.clear();
    id.clear();
    ip.ParseCommand(input);
    db.AddTable("posting_lists", ip.longestTerm() + 1 + 2*ip.size(), ip.termsCount());
    id.GeneratePostingLists(ip.size());
    freeCharArray(argv, args.size());
    id.calculateScores();
    auto vec = sortMapByValue(id.queue());
    ASSERT_EQ(vec[0].first, 1);
    ASSERT_EQ(vec[1].first, 2);
}

TEST(ScoreTEST, TEST2) {
    DataBase db("/Users/alex/labwork11-SPLOIT47/tests/ScoreTEST/TEST2");
    __lt tree;
    std::vector<std::string> args = {"labwork11", "/Users/alex/labwork11-SPLOIT47/tests/texts"};
    char** argv = convertToCharArray(args);

    InputParser ip(&db, &tree);
    Indexer id(&db, &tree);
    ip.ParseHeader(2, argv, 0);
    auto rankCount = [](size_t count){
      size_t rank = 0;
      while(count > 0) {
        ++rank;
        count /= 10;
      }
      return rank;
    };
    db.AddTable("files_path",  100, 1);
    ip.ParseHeader(2, argv, 1);
    std::string input = "biba OR boba";
    tree.clear();
    id.clear();
    ip.ParseCommand(input);
    db.AddTable("posting_lists", ip.longestTerm() + 1 + 2*ip.size(), ip.termsCount());
    id.GeneratePostingLists(ip.size());
    freeCharArray(argv, args.size());
    id.calculateScores();
    auto vec = sortMapByValue(id.queue());
    std::vector<int> dids = {1, 2, 0};
    for (int i = 0; i < vec.size(); ++i) {
        ASSERT_EQ(vec[i].first, dids[i]);
    }
}