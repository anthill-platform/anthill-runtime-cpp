
#ifndef ONLINE_Init_State_H
#define ONLINE_Init_State_H

#include "State.h"

namespace online
{
	class StateMachine;

	class InitState: public State
	{
		friend class StateMachine;

	public:
		InitState(std::shared_ptr<StateMachine> stateMachine);
		virtual ~InitState();

		virtual std::string ID() override { return "InitState"; }

	private:
		void proceed();

	protected:
		virtual void init() override;
		virtual void release() override;

	private:
	};
};

#endif