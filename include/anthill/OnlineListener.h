
#ifndef ONLINE_Listener_H
#define ONLINE_Listener_H

#include "services/LoginService.h"
#include "services/EnvironmentService.h"

namespace online
{
	typedef std::shared_ptr< class Listener > ListenerPtr;

	class Listener
	{
	public:
        virtual void multipleAccountsAttached(
            const LoginService& service,
            const LoginService::MergeOptions options,
            LoginService::MergeResolveCallback resolve) = 0;

		virtual void servicesDiscovered(std::function<void()> proceed)
		{
			proceed();
		}

		virtual void environmentVariablesReceived(const EnvironmentInformation& variables)
		{
			//
		}
	};
};

#endif
