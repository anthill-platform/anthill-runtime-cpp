
#ifndef ONLINE_State_H
#define ONLINE_State_H

#include "StateMachine.h"

namespace online
{
	typedef std::shared_ptr< class State > StatePtr;

	class StateMachine;

	class State : public std::enable_shared_from_this<State>
	{
		friend class StateMachine;

	public:
		State(std::weak_ptr<StateMachine> stateMachine);
		virtual ~State();

		virtual std::string ID() = 0;
		float getRetryTime() const;

	protected:
		virtual void init() = 0;
		virtual void release() = 0;
        virtual void failed() {}

		template <class T, typename ... ARGS>
		std::shared_ptr<T> switchTo( ARGS ... args )
		{
			if (std::shared_ptr<StateMachine> machine = m_stateMachine.lock())
			{
                if (machine->isLocked())
                {
                    return std::shared_ptr<T>(nullptr);
                }
                
				return machine->switchTo<T>( args... );
			}

			return std::shared_ptr<T>(nullptr);
		}

		bool switchTo(StatePtr to);
		void complete();
        
        // Locks the state machine from switching states
        bool lock();
        void unlock();

		void retry(int maxTries = 0);
	private:
		State(const State&) = delete;
		void operator=(const State&) = delete;
		void doubleTime();

	private:
		std::weak_ptr<StateMachine> m_stateMachine;
		float m_retryTime;
        int m_tries;
	};
};

#endif
