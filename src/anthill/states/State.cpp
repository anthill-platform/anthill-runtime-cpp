
#include "anthill/states/State.h"
#include "anthill/states/RetryState.h"
#include "anthill/states/StateMachine.h"

#include "anthill/Log.h"

namespace online
{
	State::State(std::weak_ptr<StateMachine> stateMachine) :
		m_stateMachine(stateMachine),
		m_retryTime(0),
        m_tries(-1)
	{
		
	}

	void State::complete()
	{
		if (std::shared_ptr<StateMachine> machine = m_stateMachine.lock())
		{
			machine->complete();
		}
	}

	void State::retry(int maxTries)
	{
		Log::get() << "Retrying in " << getRetryTime() << std::endl;

		StatePtr ptr = shared_from_this();

        if (m_tries == -1)
        {
            m_tries = maxTries;
        }
        else
        {
            if (m_tries > 0)
            {
                m_tries--;
                
                if (m_tries <= 0)
                {
                    failed();
                    
                    return;
                }
            }
        }
        
		switchTo<RetryState>(ptr);
		doubleTime();
	}

	void State::doubleTime()
	{
		if (m_retryTime > 0)
		{
			m_retryTime *= 2.0f;

			if (m_retryTime > RetryState::MaxRetryTime)
			{
				m_retryTime = RetryState::MaxRetryTime;
			}
		}
		else
		{
			m_retryTime = 1.0f;
		}
	}

	float State::getRetryTime() const
	{
		return m_retryTime;
	}
    
    bool State::lock()
    {
		if (std::shared_ptr<StateMachine> machine = m_stateMachine.lock())
		{
            return machine->lock();
		}
        
        return false;
    }
    
    void State::unlock()
    {
		if (std::shared_ptr<StateMachine> machine = m_stateMachine.lock())
		{
            machine->unlock();
		}
    }

	bool State::switchTo(StatePtr to)
	{
		if (std::shared_ptr<StateMachine> machine = m_stateMachine.lock())
		{
            return machine->switchTo(to);
		}
        
        return false;
	}

	State::~State()
	{
		//
	}
}
