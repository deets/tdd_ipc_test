#include <string>
#include "gtest/gtest.h"

#include "ipc_server.hh"

using namespace std;

namespace {

// The fixture for testing class Server.
class ServerTest : public ::testing::Test {
 protected:
  // You can remove any or all of the following functions if its body
  // is empty.

  ServerTest() {
    // You can do set-up work for each test here.
  }

  virtual ~ServerTest() {
    // You can do clean-up work that doesn't throw exceptions here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:

  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }

  // Objects declared here can be used by all tests in the test case for Server.
};

// Tests that the Server::Bar() method does Abc.
TEST_F(ServerTest, MethodBarDoesAbc) {
  const string input_filepath = "this/package/testdata/myinputfile.dat";
  const string output_filepath = "this/package/testdata/myoutputfile.dat";
  //Server f;
  EXPECT_EQ(0, 0); //f.Bar(input_filepath, output_filepath));
}

// Tests that Server does Xyz.
TEST_F(ServerTest, DoesXyz) {
  // Exercises the Xyz feature of Server.
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
