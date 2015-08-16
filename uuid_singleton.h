#ifndef _UUID_SINGLETON_H
#define _UUID_SINGLETON_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>

class UUIDLogger
{
public:
	// Returns a reference to the singleton UUIDLogger object
	static UUIDLogger& instance();
	// Write uuid to logger
	void log(const std::string& uuid);
	// Setting output file name
	static void setFileName(std::string fname);
private:
	// Output filename
	static std::string m_uuidFileName;
	// Static variable for the one-and-only instance
	static UUIDLogger* m_pInstance;
	// Data member for the output stream
	std::ofstream m_outputStream;
	// Embedded class to make sure the single Logger instance gets deleted on program shutdown.
	friend class Cleanup;
	class Cleanup
	{
	public:
		~Cleanup();
	};
	// Write uuid to log stream. The thread should own a lock on sMutex before calling this function.
	void writeUUID(const std::string& uuid);

	UUIDLogger();
	virtual ~UUIDLogger();
	UUIDLogger(const UUIDLogger&);
	UUIDLogger& operator=(const UUIDLogger&);
	static std::mutex sMutex;
};

#endif
