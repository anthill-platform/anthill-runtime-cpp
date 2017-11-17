
#ifndef ONLINE_Environment_Service_H
#define ONLINE_Environment_Service_H

#include "Service.h"
#include "../requests/Request.h"
#include "../ApplicationInfo.h"
#include <json/value.h>
#include "DiscoveryService.h"

namespace online
{
	typedef std::shared_ptr< class EnvironmentService > EnvironmentServicePtr;
	typedef Json::Value EnvironmentInformation;

	class AnthillRuntime;

	class EnvironmentService : public Service
	{
		friend class AnthillRuntime;

	public:
		static const std::string ID;
        static const std::string API_VERSION;

		typedef std::function< void(const EnvironmentService&, Request::Result result, const Request& request,
			const std::string& discoveryLocation, const EnvironmentInformation&) > EnvironmentInfoCallback;

	public:
		static EnvironmentServicePtr Create(const std::string& location);
		virtual ~EnvironmentService();

		void getEnvironmentInfo(EnvironmentInfoCallback callback);
		void setDiscoveryLocation(const std::string& location);

		DiscoveryServicePtr getDiscoveryService() const;

	protected:
		EnvironmentService(const std::string& location);
		bool init();

	private:
		DiscoveryServicePtr m_discovery;
	};
};

#endif
