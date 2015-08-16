#include <stdexcept>
#include "uuid_singleton.h"

std::string UUIDLogger::m_uuidFileName = "uuid.txt";
UUIDLogger* UUIDLogger::m_pInstance = nullptr;
std::mutex UUIDLogger::sMutex;

void UUIDLogger::setFileName(std::string fname)
{
	if (m_pInstance == nullptr)
	{
		m_uuidFileName = fname;
	}
}

UUIDLogger& UUIDLogger::instance()
{
	static Cleanup cleanup;
	std::lock_guard<std::mutex> guard(sMutex);
	if (m_pInstance == nullptr)
	{
		m_pInstance = new UUIDLogger();
	}
	return *m_pInstance;
}

UUIDLogger::Cleanup::~Cleanup()
{
	std::lock_guard<std::mutex> guard(UUIDLogger::sMutex);
	delete UUIDLogger::m_pInstance;
	UUIDLogger::m_pInstance = nullptr;
}

UUIDLogger::~UUIDLogger()
{
	m_outputStream.close();
}

UUIDLogger::UUIDLogger()
{
	m_outputStream.open(m_uuidFileName, std::ios_base::app);
	if (!m_outputStream.good())
	{
		throw std::runtime_error("Unable to initialize the UUIDLogger!");
	}
}

void UUIDLogger::log(const std::string& uuid)
{
	std::lock_guard<std::mutex> guard(sMutex);
	writeUUID(uuid);
}

void UUIDLogger::writeUUID(const std::string& uuid)
{
	m_outputStream << uuid;
}
