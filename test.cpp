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

  ServerTest() : pipe_name("test_server_pipe_name")
  {
    // You can do set-up work for each test here.
    _server = new Server(pipe_name);
  }

  virtual ~ServerTest() {
    delete _server;
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

  // Objects declared here can be used by all tests in the test case for
  // Server.

  const string pipe_name;

  Server *_server;
};

// Tests that the Server::Bar() method does Abc.
TEST_F(ServerTest, ConstructionWithPipeName) {


  EXPECT_EQ(pipe_name, _server->name());
}

// Tests that sending string messages works
TEST_F(ServerTest, SendMessage) {
  _server->send("foobar");
}

}  // namespace

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
