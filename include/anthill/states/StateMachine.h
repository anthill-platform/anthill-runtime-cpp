
#ifndef ONLINE_StateMachine_H
#define ONLINE_StateMachine_H

#include <functional>
#include <memory>
#include <string>

namespace online
{
	typedef std::shared_ptr< class StateMachine > StateMachinePtr;
	typedef std::shared_ptr< class StateMachine > StateMachineWPtr;
    
    typedef std::shared_ptr< class State > StatePtr;
	
	class StateMachine: public std::enable_shared_from_this<StateMachine>
	{
	public:
		typedef std::function< void() > CompleteCallback;

	public:
		static StateMachinePtr Create(const std::string& name);
		virtual ~StateMachine();

		StatePtr getCurrentState();

		template <class T, typename ... ARGS>
		std::shared_ptr<T> switchTo( ARGS ... args)
		{
            if (m_locked)
                return nullptr;
			std::shared_ptr<T> ptr = std::make_shared<T>(shared_from_this(), args...);
			switchTo(std::static_pointer_cast<State>(ptr));
			return ptr;
		}

		bool switchTo(StatePtr to);
		void complete();

		std::string getName() const;
        
        // Locks the state machine from switching states
        bool lock();
        void unlock();
        
        inline bool isLocked() const { return m_locked; }

		void setOnComplete(CompleteCallback callback);
	protected:
		StateMachine(const std::string& name);
		bool init();

	private:
		StateMachine(const StateMachine&) = delete;
		void operator=(const StateMachine&) = delete;

	private:
		StatePtr m_currentState;
		CompleteCallback m_completeCallback;
		std::string m_name;
        bool m_locked;
	};
};

#endif
