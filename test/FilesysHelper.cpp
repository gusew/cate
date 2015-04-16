#include <libunittest/all.hpp>
#include <frontend/FilesysHelper.hpp>
#include <string>

using namespace unittest::assertions;

TEST(test_filesyshelper_parsepath)
{
  std::string resPre, resPost;
  FilesysHelper::parsePath("", resPre, resPost);
  assert_equal(resPre, ".", SPOT);
  assert_equal(resPost, "cate", SPOT);

  FilesysHelper::parsePath("/", resPre, resPost);
  assert_equal(resPre, "/", SPOT);
  assert_equal(resPost, "", SPOT);

  FilesysHelper::parsePath("/program", resPre, resPost);
  assert_equal(resPre, "/", SPOT);
  assert_equal(resPost, "program", SPOT);

  FilesysHelper::parsePath("/pRog Ram", resPre, resPost);
  assert_equal(resPre, "/", SPOT);
  assert_equal(resPost, "pRog Ram", SPOT);

  FilesysHelper::parsePath("program", resPre, resPost);
  assert_equal(resPre, ".", SPOT);
  assert_equal(resPost, "program", SPOT);

  FilesysHelper::parsePath("path/program", resPre, resPost);
  assert_equal(resPre, "path/", SPOT);
  assert_equal(resPost, "program", SPOT);

  FilesysHelper::parsePath("./path/program", resPre, resPost);
  assert_equal(resPre, "./path/", SPOT);
  assert_equal(resPost, "program", SPOT);

  FilesysHelper::parsePath("./folder/sub-folder/deep er/program-file.exe", resPre, resPost);
  assert_equal(resPre, "./folder/sub-folder/deep er/", SPOT);
  assert_equal(resPost, "program-file.exe", SPOT);
}

TEST(test_filesyshelper_checkdir)
{
  assert_true(FilesysHelper::checkDirExists("."), SPOT);
  assert_true(FilesysHelper::checkDirExists(".."), SPOT);
  assert_true(FilesysHelper::checkDirExists("build"), SPOT);
  assert_true(FilesysHelper::checkDirExists("./build"), SPOT);
  assert_true(FilesysHelper::checkDirExists("./build/../."), SPOT);
  assert_true(FilesysHelper::checkDirExists("./build/../build"), SPOT);
  assert_true(FilesysHelper::checkDirExists("build/test"), SPOT);
  assert_false(FilesysHelper::checkDirExists("fantasy"), SPOT);

  assert_true(FilesysHelper::checkDirExists("/usr/bin"), SPOT);
  assert_false(FilesysHelper::checkDirExists("/tmp/sub/dir/fantasy"), SPOT);
}

TEST(test_filesyshelper_checkfile)
{
  assert_true(FilesysHelper::checkFileExists("makefile"), SPOT);
  assert_true(FilesysHelper::checkFileExists("./build/test/cate_unittest"), SPOT);
  assert_true(FilesysHelper::checkFileExists("/usr/bin/make"), SPOT);

  assert_false(FilesysHelper::checkFileExists("fantasy-file"), SPOT);
  assert_false(FilesysHelper::checkFileExists("./fantasy-file"), SPOT);
  assert_false(FilesysHelper::checkFileExists("./build/fantasy-file"), SPOT);
}

TEST(test_filesyshelper_checkandsetdir)
{
  std::string dir;

  dir = ".";
  assert_true(FilesysHelper::checkAndSetDir(dir, ""), SPOT);
  assert_equal(dir, ".", SPOT);
  dir = "core";
  assert_true(FilesysHelper::checkAndSetDir(dir, "./include/"), SPOT);
  assert_equal(dir, "./include/core", SPOT);

  dir = "/usr/bin";
  assert_true(FilesysHelper::checkAndSetDir(dir, ""), SPOT);
  assert_equal(dir, "/usr/bin", SPOT);
  assert_true(FilesysHelper::checkAndSetDir(dir, "./../"), SPOT);
  assert_equal(dir, "/usr/bin", SPOT);
}

TEST(test_filesyshelper_readfile)
{
  std::string result;
  assert_true(FilesysHelper::readFile("makefile", result), SPOT);
  assert_greater(result.size(), (unsigned)0, SPOT);
}

