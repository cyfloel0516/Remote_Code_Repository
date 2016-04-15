/////////////////////////////////////////////////////////////////////////////
// ClientTest.cpp - Test passing strings, blocks, and buffers              //
// Ver 1.3                                                                 //
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
*  devenv Client.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.3 : 04 Mar 16
*  - many changes to make starting and ending tests more reliable
*  - most of this simplified the signaling between client and server
*    about test initiation and completion.
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

using Util = Utilities::StringHelper;

template<typename T>
using Conv = Utilities::Converter<T>;
using Show = StaticLogger<1>;

//----< test string handling - server echos back client sent string >--------

void clientTestStringHandling(Socket& si)
{
  std::string command = "TEST_STRING_HANDLING\nDone";
  si.sendString(command);
  Show::write("\n  client sent : " + command);

  for (size_t i = 0; i < 5; ++i)
  {
    std::string text = "Hello World " + std::string("#") + Conv<size_t>::toString(i + 1);
    si.sendString(text);
    Show::write("\n  client sent : " + text);
  }
  command = "TEST_END";
  si.sendString(command);
  Show::write("\n  client sent : " + command);

  while (true)
  {
    std::string str = si.recvString();
    if (str.size() == 0)
    {
      Show::write("\n  client detected closed connection");
      break;
    }
    Show::write("\n  client recvd: " + str);
    if (str == "TEST_END")
    {
      Show::write("\n  End of string handling test");
      break;
    }
  }
}
//----< test buffer handling - server echos back client sent buffer >--------

void clientTestBufferHandling(Socket& si)
{
  std::string command = "TEST_BUFFER_HANDLING";
  si.sendString(command);
  Show::write("\n  client sent : " + command);

  const int BufLen = 20;
  Socket::byte buffer[BufLen];

  for (size_t i = 0; i < 5; ++i)
  {
    std::string text = "Hello World " + std::string("#") + Conv<size_t>::toString(i + 1);
    for (size_t i = 0; i < BufLen; ++i)
    {
      if (i < text.size())
        buffer[i] = text[i];
      else
        buffer[i] = '.';
    }
    buffer[BufLen - 1] = '\0';
    si.send(BufLen, buffer);
    Show::write("\n  client sent : " + std::string(buffer));
  }
  std::string text = "TEST_END";
  for (size_t i = 0; i < BufLen; ++i)
  {
    if (i < text.size())
      buffer[i] = text[i];
    else
      buffer[i] = '.';
  }
  buffer[BufLen - 1] = '\0';
  si.send(BufLen, buffer);
  Show::write("\n  client sent : " + std::string(buffer));

  bool ok;
  while (true)
  {
    ok = si.recv(BufLen, buffer);
    if (!ok)
    {
      Show::write("\n  client unable to receive");
      break;
    }
    std::string str(buffer);
    if (str.size() == 0)
    {
      Show::write("\n  client detected closed connection");
      break;
    }
    Show::write("\n  client rcvd : " + str);
    if (str.find("TEST_END") != std::string::npos)
    {
      Show::write("\n  End of buffer handling test");
      break;
    }
  }
}
//----< demonstration >------------------------------------------------------

int main()
{
  Show::attach(&std::cout);
  Show::start();
  Show::title("Testing Socket Client", '=');

  try
  {
    SocketSystem ss;
    SocketConnecter si;
    while (!si.connect("localhost", 9080))
    {
      Show::write("\n  client waiting to connect");
      ::Sleep(100);
    }
    Show::write("\n");
    Show::title("Starting string test on client");
    clientTestStringHandling(si);

    Show::write("\n");
    Show::title("Starting buffer test on client");
    clientTestBufferHandling(si);

    Show::write("\n");
    Show::title("Testing complete");

    si.sendString("Done");
    Show::write("\n  client sent Done");

    std::string reply = si.recvString();
    Show::write("\n  client rcvd : " + reply);

    Show::write("\n\n  client calling send shutdown\n");
    si.shutDownSend();
  }
  catch (std::exception& ex)
  {
    Show::write("\n  Exception caught:");
    Show::write(std::string("\n\n  ") + ex.what() + "\n\n");
  }
}
