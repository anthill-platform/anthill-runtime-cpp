
#include "anthill/states/GetEnvironmentState.h"
#include "anthill/states/DiscoverServicesState.h"
#include "anthill/states/StateMachine.h"

#include "anthill/Log.h"
#include "anthill/Utils.h"
#include "anthill/AnthillRuntime.h"

using namespace std::placeholders;

namespace online
{
	GetEnvironmentState::GetEnvironmentState(std::shared_ptr<StateMachine> stateMachine) :
		State(stateMachine)
	{
	}

	void GetEnvironmentState::environmentInformation(const EnvironmentService& env, Request::Result result, const Request& request,
		const std::string& discoveryLocation, const EnvironmentInformation& data)
	{
        unlock();
    
		if (Request::isSuccessful(result))
		{
			Log::get() << "GetEnvironmentInfo successful!" << std::endl;

			EnvironmentServicePtr ptr = AnthillRuntime::Instance().get<EnvironmentService>();
			OnlineAssert((bool)ptr, "Environment service is not initialized!");

			ptr->setDiscoveryLocation(discoveryLocation);
			
			AnthillRuntime& online = AnthillRuntime::Instance();
			const ListenerPtr& listener = online.getListener();
			if (listener)
			{
				listener->environmentVariablesReceived(data);
			}

			proceed();
		}
		else
		{
			Log::get() << "GetEnvironmentInfo failed: " << result << ": " << request.getResponseAsString() << std::endl;

			retry();
		}
	}

	void GetEnvironmentState::proceed()
	{
		switchTo<DiscoverServicesState>(AnthillRuntime::Instance().getEnabledServices());
	}

	void GetEnvironmentState::init()
	{
		EnvironmentServicePtr ptr = AnthillRuntime::Instance().get<EnvironmentService>();

		OnlineAssert((bool)ptr, "Environment service is not initialized!");
        
        lock();

		ptr->getEnvironmentInfo(
			std::bind(&GetEnvironmentState::environmentInformation, this, _1, _2, _3, _4, _5));
	}

	void GetEnvironmentState::release()
	{
		//
	}

	GetEnvironmentState::~GetEnvironmentState()
	{
		//
	}
}
