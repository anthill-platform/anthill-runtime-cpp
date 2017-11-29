
#ifndef ONLINE_Authenticate_State_H
#define ONLINE_Authenticate_State_H

#include "State.h"
#include "../services/LoginService.h"

#include <set>

namespace online
{
	class StateMachine;

	class AuthenticateState : public State
	{
		friend class StateMachine;

	public:
		AuthenticateState(std::shared_ptr<StateMachine> stateMachine, 
			const std::string& username,
			const std::string& password, 
			const std::string& gamespace,
			const LoginService::Scopes& scopes,
            std::shared_ptr<State> returnState = nullptr,
            const LoginService::Scopes& shouldHaveScopes = {"*"});

		virtual ~AuthenticateState();

		virtual std::string ID() override { return "AuthenticateState"; }

	protected:
		virtual void init() override;
		virtual void release() override;

		void authenticateResult(const LoginService& service, Request::Result result, const Request& request, const std::string& accessToken,
			 const std::string& credential,  const std::string& account, const LoginService::Scopes& scopes);
        void merge(const LoginService& service, const LoginService::MergeOptions options, LoginService::MergeResolveCallback resolve);

	private:
		void proceed();

	private:
		std::string m_username;
		std::string m_password;
		std::string m_gamespace;
		LoginService::Scopes m_scopes;
        LoginService::Scopes m_shouldHaveScopes;
        
		std::shared_ptr<State> m_returnState;
	};
};

#endif
