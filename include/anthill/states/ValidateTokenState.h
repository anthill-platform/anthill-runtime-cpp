
#ifndef ONLINE_ValidateToken_State_H
#define ONLINE_ValidateToken_State_H

#include "State.h"
#include "../services/LoginService.h"

#include <set>

namespace online
{
	class StateMachine;

	class ValidateTokenState : public State
	{
		friend class StateMachine;

	public:
		ValidateTokenState(std::shared_ptr<StateMachine> stateMachine, const std::string& accessToken);
		virtual ~ValidateTokenState();

		virtual std::string ID() override { return "ValidateTokenState"; }

	protected:
		virtual void init() override;
		virtual void release() override;

	private:
		void proceed();

	private:
		std::string m_accessToken;
	};
};

#endif