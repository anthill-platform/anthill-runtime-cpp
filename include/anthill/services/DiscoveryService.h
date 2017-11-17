
#ifndef ONLINE_Discovery_Service_H
#define ONLINE_Discovery_Service_H

#pragma warning(disable : 4503)

#include "Service.h"
#include "../requests/Request.h"
#include "../ApplicationInfo.h"
#include <json/value.h>

#include <set>
#include <unordered_map>

namespace online
{
	typedef std::shared_ptr< class DiscoveryService > DiscoveryServicePtr;
	typedef std::unordered_map< std::string, ServicePtr > DiscoveredServices;

	class AnthillRuntime;

	class DiscoveryService : public Service
	{
		friend class AnthillRuntime;

	public:
		static const std::string ID;
        static const std::string API_VERSION;

		typedef std::function< void(const  DiscoveryService&, Request::Result result, const Request& request,
			const DiscoveredServices& services) > DiscoveryInfoCallback;

	public:
		static DiscoveryServicePtr Create(const std::string& location);
		virtual ~DiscoveryService();

		void discoverServices(const std::set<std::string>& services, DiscoveryInfoCallback callback);
	protected:
        
		DiscoveryService(const std::string& location);
        bool init();
        void setServices(const Json::Value& services);

	private:
		DiscoveredServices m_services;
	};
};

#endif
