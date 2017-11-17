
#include "anthill/states/InitState.h"
#include "anthill/states/GetEnvironmentState.h"
#include "anthill/states/StateMachine.h"

#include "anthill/Log.h"

namespace online
{
	InitState::InitState(std::shared_ptr<StateMachine> stateMachine) :
		State(stateMachine)
	{
        
	}

	void InitState::init()
	{
        Log::get() << "Online lib initialization... " << std::endl;
        
		proceed();
	}

	void InitState::proceed()
	{
		switchTo<GetEnvironmentState>();
	}

	void InitState::release()
	{
		//
	}

	InitState::~InitState()
	{
		//
	}
}