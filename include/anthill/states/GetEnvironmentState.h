
#ifndef ONLINE_GetEnvironment_State_H
#define ONLINE_GetEnvironment_State_H

#include "State.h"
#include "../services/EnvironmentService.h"

namespace online
{
	class StateMachine;

	class GetEnvironmentState: public State
	{
		friend class StateMachine;

	public:
		GetEnvironmentState(std::shared_ptr<StateMachine> stateMachine);
		virtual ~GetEnvironmentState();

		virtual std::string ID() override { return "GetEnvironmentState"; }

	protected:
		virtual void init() override;
		virtual void release() override;

	private:
		void proceed();

	private:
		void environmentInformation(const EnvironmentService& env, Request::Result result, 
			const Request& request, const std::string& discoveryLocation, const EnvironmentInformation& data);
	};
};

#endif