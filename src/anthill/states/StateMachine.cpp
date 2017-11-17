
#include "anthill/states/StateMachine.h"
#include "anthill/states/State.h"

#include "anthill/Log.h"

namespace online
{
	StateMachinePtr StateMachine::Create(const std::string& name)
	{
		StateMachinePtr _object(new StateMachine(name));
		if (!_object->init())
			return StateMachinePtr(nullptr);

		return _object;
	}

	bool StateMachine::init()
	{
		return true;
	}

	void StateMachine::setOnComplete(CompleteCallback callback)
	{
		m_completeCallback = callback;
	}
	
	void StateMachine::complete()
	{
		if (m_completeCallback)
		{
			m_completeCallback();
		}

		switchTo(nullptr);
	}

	std::string StateMachine::getName() const
	{
		return m_name;
	}

	bool StateMachine::switchTo(StatePtr to)
	{
        if (m_locked)
        {
            return false;
        }
        
		if (m_currentState)
		{
			m_currentState->release();
		}

		m_currentState = to;

		if (m_currentState)
		{
			Log::get() << getName() << ": Switched to state '" << m_currentState->ID() << "'." << std::endl;

			m_currentState->init();
		}
        
        return true;
	}

    bool StateMachine::lock()
    {
        if (m_locked)
            return false;
        
        m_locked = true;
        return true;
    }
    
    void StateMachine::unlock()
    {
        m_locked = false;
    }

	StatePtr StateMachine::getCurrentState()
	{
		return m_currentState;
	}

	StateMachine::StateMachine(const std::string& name) :
		m_currentState(nullptr),
		m_name(name),
        m_locked(false)
	{
	}

	StateMachine::~StateMachine()
	{
	}
}
