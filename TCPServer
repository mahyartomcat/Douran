//
// TimeServer.cpp
//
// $Id: //poco/1.4/Net/samples/TimeServer/src/TimeServer.cpp#1 $
//
// This sample demonstrates the TCPServer and ServerSocket classes.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include <Poco/UUID.h>
#include <Poco/UUIDGenerator.h>

#include "Poco/Net/TCPServer.h"
#include "Poco/Net/TCPServerConnection.h"
#include "Poco/Net/TCPServerConnectionFactory.h"
#include "Poco/Net/TCPServerParams.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/Exception.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include <iostream>

using Poco::UUID;
using Poco::UUIDGenerator;

using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::Net::TCPServerConnection;
using Poco::Net::TCPServerConnectionFactory;
using Poco::Net::TCPServer;
using Poco::Net::TCPServerParams;

using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;


class UUID_TCPServerConnection : public TCPServerConnection
	/// This class handles all client connections.
	///
	/// A string of uuid is sent back to the client.
{
public:
	UUID_TCPServerConnection(const StreamSocket& s) : TCPServerConnection(s) {}

	void run()
	{
		StreamSocket& ss = socket();
		Application& app = Application::instance();
		app.logger().information("Request from " + ss.peerAddress().toString());
		try
		{
			UUIDGenerator& generator = UUIDGenerator::defaultGenerator();
			UUID uuid1(generator.create());					// time based
			std::string dt(uuid1.toString());
			// FIXME: in linux use \r\n
			dt.append("\n");
			ss.sendBytes(dt.data(), (int)dt.length());
		}
		catch (Poco::Exception& exc)
		{
			app.logger().log(exc);
		}
	}
};


class UUID_TCPServerConnectionFactory : public TCPServerConnectionFactory
	/// A factory for UUID_TCPServerConnection.
{
public:
	UUID_TCPServerConnectionFactory() {}

	TCPServerConnection* createConnection(const StreamSocket& socket)
	{
		return new UUID_TCPServerConnection(socket);
	}
};


class UUID_TCPServer : public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the UUID_TCPServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (TCPServer.properties),
	/// copy the file to the directory where the TCPServer executable
	/// resides. If you start the debug version of the TCPServer
	/// (TCPServerd[.exe]), you must also create a copy of the configuration
	/// file named TimeServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9911).
	///
	/// To test the TCPServer you can use any telnet client (telnet localhost 9911).
{
public:
	UUID_TCPServer() : _helpRequested(false)
	{
	}

	~UUID_TCPServer()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}

	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);

		options.addOption(
			Option("help", "h", "display help information on command line arguments")
			.required(false)
			.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("A server application that generates uuid");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			// get parameters from configuration file
			unsigned short port = (unsigned short)config().getInt("TCPServer.port", 9911);
			////std::string format(config().getString("TCPServer.format"));

			// set-up a server socket
			ServerSocket svs(port);

			//FIXME: for limiting max number of threads following commands can be used:
			//TCPServerParams* pParams = new TCPServerParams;
			//pParams->setMaxThreads(8);
			//pParams->setMaxQueued(4);
			//pParams->setThreadIdleTime(100);

			// set-up a TCPServer instance
			TCPServer srv(new UUID_TCPServerConnectionFactory(), svs);// , pParams);
			// start the TCPServer
			srv.start();
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the TCPServer
			srv.stop();
		}
		return Application::EXIT_OK;
	}

private:
	bool _helpRequested;
};


int main(int argc, char** argv)
{
	UUID_TCPServer app;
	return app.run(argc, argv);
}
