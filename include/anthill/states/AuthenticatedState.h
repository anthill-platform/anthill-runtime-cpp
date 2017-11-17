
#ifndef ONLINE_Authenticated_State_H
#define ONLINE_Authenticated_State_H

#include "State.h"

namespace online
{
	class StateMachine;

	class AuthenticatedState : public State
	{
		friend class StateMachine;

	public:
		AuthenticatedState(std::shared_ptr<StateMachine> stateMachine);
		virtual ~AuthenticatedState();

	protected:
		void authenticate();
	private:
	};

};

#endif