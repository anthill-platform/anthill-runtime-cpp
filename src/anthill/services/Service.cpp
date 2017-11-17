
#include "anthill/services/Service.h"

namespace online
{
	const std::string& Service::getLocation() const
	{
		return m_location;
	}

	Service::Service(const std::string& location) : 
		m_location(location) 
	{
		//
	}

	bool Service::init()
	{
		return true;
	}

	void Service::setLocation(const std::string& location)
	{
		m_location = location;
	}
}
