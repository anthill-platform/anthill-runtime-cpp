
#ifndef ONLINE_GenerateUser_State_H
#define ONLINE_GenerateUser_State_H

#include "State.h"
#include "RetryState.h"
#include "../services/LoginService.h"

#include <set>

namespace online
{
	class StateMachine;

	class GenerateUserState : public State
	{
		friend class StateMachine;

	public:
		GenerateUserState(std::shared_ptr<StateMachine> stateMachine);
		virtual ~GenerateUserState();

		virtual std::string ID() override { return "GenerateUserState"; }

	private:
		void proceed();

	protected:
		virtual void init() override;
		virtual void release() override;

	private:
	};
};

#endif