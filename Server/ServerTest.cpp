/////////////////////////////////////////////////////////////////////////////
// ServerTest.cpp - Test passing strings, blocks, and buffers              //
// Ver 1.4                                                                 //
// Jim Fawcett, CSE#687 - Object Oriented Design, Spring 2016              //
// CST 4-187, Syracuse University, 315 443-3948, jfawcett@twcny.rr.com     //
//-------------------------------------------------------------------------//
// Jim Fawcett (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Demo for Project #4, Communication Channel                 //
// Platform:    Dell XPS 8900, Windows 10 Pro, Visual Studio 2015          //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides tests for Sockets:
*  - passing byte buffers
*  - passing strings
*  It uses a single socket connection using full duplex communication between
*  a client and server.  This kind of communication has to be very carefully
*  handled, essentially using a talk token to determine when to send and
*  when to receive.  In this package the tokens are commands used to start
*  testing and to indicate the end of tests.
*
*  Required Files:
*  ---------------
*  ClientTest.cpp, ServerTest.cpp
*  Sockets.h, Sockets.cpp,
*  Logger.h, Logger.cpp,
*  Utilities.h, Utilities.cpp,
*
*  Build Command:
*  --------------
*  devenv Server.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.4 : 04 Mar 16
*  - many changes to make starting and ending tests more reliable
*  - most of this simplified the signaling between client and server
*    about test initiation and completion.
*  ver 1.3 : 29 Mar 16
*  - Replaced socket_.bytesWaiting() with !socket_.waitForData(1000,10)
*    in ClientHandler::testBufferHandling(Socket&)
*  ver 1.2 : 22 Feb 16
*  - Replaced verbose I/O with Logger I/O
*  - Replaced ApplicationHelpers package with Utilities package
*  ver 1.1 : 30 Mar 15
*  - minor modification to comments, above
*  ver 1.0 : 30 Mar 15
*  - first release
*/
#include "../sockets/sockets.h"
#include "../Logger/Logger.h"
#include "../Utilities/Utilities.h"
#include <iostream>
#include <thread>
#include <mutex>

using Util = Utilities::StringHelper;

template<typename T>
using Conv = Utilities::Converter<T>;
using Show = StaticLogger<1>;

/////////////////////////////////////////////////////////////////////////////
// Server's client handler class
// - must be callable object so we've built as a functor
// - passed to SocketListener::start(CallObject& co)
// - Client handling thread starts by calling operator()

class ClientHandler
{
public:
  void operator()(Socket& socket_);
  bool testStringHandling(Socket& socket_);
  bool testBufferHandling(Socket& socket_);
};

//----< Client Handler thread starts running this function >-----------------

void clearBuffer(Socket::byte* buffer, size_t BufLen)
{
  for (size_t i = 0; i < BufLen; ++i)
    buffer[i] = '\0';
}

void ClientHandler::operator()(Socket& socket_)
{
  while (true)
  {
    // interpret test command
    std::string command = socket_.recvString();
    Show::write("\n  server rcvd command: " + command);
    if (command == "Done")
    {
      Show::write("\n  server sent : " + command);
      socket_.sendString(command);
      break;
    }
    if (command.size() == 0)
    {
      Show::write("\n  client connection closed");
      break;
    }
    //Show::write("\n  server recvd: " + command);

    if (command == "TEST_STRING_HANDLING")
    {
      if (testStringHandling(socket_))
        Show::write("\n  ----String Handling test passed\n");
      else
        Show::write("\n  ----String Handling test failed\n");
      continue; // go back and get another command
    }
    if (command == "TEST_BUFFER_HANDLING")
    {
      if (testBufferHandling(socket_))
        Show::write("\n  ----Buffer Handling test passed\n");
      else
        Show::write("\n  ----Buffer Handling test failed\n");
      continue;  // get another command
    }
  }

  // we get here if command isn't requesting a test, e.g., "TEST_STOP"

  Show::write("\n");
  Show::write("\n  ClientHandler socket connection closing");
  socket_.shutDown();
  socket_.close();
  Show::write("\n  ClientHandler thread terminating");
}

//----< test string handling >-----------------------------------------------
/*
*   Creates strings, sends to server, then reads strings server echos back.
*/
bool ClientHandler::testStringHandling(Socket& socket_)
{
  Show::title("String handling test on server");

  while (true)
  {
    std::string str = socket_.recvString();
    if (socket_ == INVALID_SOCKET)
      return false;
    if (str.size() > 0)
    {
      //Show::write("\n  bytes recvd at server: " + toString(str.size() + 1));
      Show::write("\n  server rcvd : " + str);

      if (socket_.sendString(str))
      {
        Show::write("\n  server sent : " + str);
      }
      else
      {
        return false;
      }
      if (str == "TEST_END")
        break;
    }
    else
    {
      break;
    }
  }
  //socket_.sendString("TEST_STRING_HANDLING_END");
  Show::write("\n  End of string handling test");
  return true;
}

//----< test buffer handling >-----------------------------------------------
/*
*   Creates buffers, sends to server, then reads buffers server echos back.
*/
bool ClientHandler::testBufferHandling(Socket& socket_)
{
  Show::title("Buffer handling test on server");
  const size_t BufLen = 20;
  Socket::byte buffer[BufLen];
  bool ok;

  while (true)
  {
    ok = socket_.recv(BufLen, buffer);
    if (socket_ == INVALID_SOCKET)
      return false;
    if (ok)
    {
      std::string temp;
      for (size_t i = 0; i < BufLen; ++i)
        temp += buffer[i];
      //Show::write("\n  bytes recvd at server: " + toString(BufLen));
      Show::write("\n  server rcvd : " + temp);

      if (socket_.send(BufLen, buffer))
      {
        Show::write("\n  server sent : " + std::string(buffer));
      }
      else
      {
        Show::write("\n  server send failed");
        return false;
      }
      if (temp.find("TEST_END") != std::string::npos)
        break;
    }
    else
    {
      break;
    }
  }
  Show::write("\n  End of buffer handling test");
  return true;
}

//----< test stub starts here >----------------------------------------------

int main()
{
  Show::attach(&std::cout);
  Show::start();
  Show::title("Testing Socket Server", '=');

  try
  {
    SocketSystem ss;
    SocketListener sl(9080, Socket::IP6);
    ClientHandler cp;
    sl.start(cp);

    Show::write("\n -------------------\n  press key to exit: \n -------------------");
    std::cout.flush();
    std::cin.get();
  }
  catch (std::exception& ex)
  {
    Show::write("\n  Exception caught:");
    Show::write("\n  " + std::string("\n  ") + ex.what() + "\n\n");
  }
}

