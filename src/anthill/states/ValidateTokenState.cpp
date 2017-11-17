
#include "anthill/states/ValidateTokenState.h"
#include "anthill/states/CheckUserState.h"
#include "anthill/states/AuthenticateState.h"
#include "anthill/states/StateMachine.h"

#include "anthill/Log.h"
#include "anthill/Utils.h"
#include "anthill/AnthillRuntime.h"

using namespace std::placeholders;

namespace online
{
	ValidateTokenState::ValidateTokenState(std::shared_ptr<StateMachine> stateMachine, const std::string& accessToken) :
		State(stateMachine),
		m_accessToken(accessToken)
	{
	}

	void ValidateTokenState::init()
	{
		LoginServicePtr ptr = AnthillRuntime::Instance().get<LoginService>();
		OnlineAssert((bool)ptr, "Login service is not initialized!");
        
        lock();

		ptr->validateAccessToken(m_accessToken, [this](const LoginService& service, Request::Result result, const Request& request,
            const std::string& credential, const std::string& account, const LoginService::Scopes& scopes)
		{
            unlock();
        
			if (request.isSuccessful())
			{
                const ApplicationInfo& applicationInfo = AnthillRuntime::Instance().getApplicationInfo();

                // check if the requiredScopes are satisfied
                if (std::includes(scopes.begin(), scopes.end(), applicationInfo.requiredScopes.begin(), applicationInfo.requiredScopes.end()))
                {
                    Log::get() << "Access token is valid!" << std::endl;

                    proceed();
                    return;
                }
			}
   
            Log::get() << "Access token is invalid!" << std::endl;

            AnthillRuntime& online = AnthillRuntime::Instance();
            StoragePtr storage = online.getStorage();

            storage->remove(CheckUserState::StorageAccessTokeneField);
            storage->save();

            switchTo<CheckUserState>();
		});
	}

	void ValidateTokenState::proceed()
    {
        complete();
	}

	void ValidateTokenState::release()
	{
		//
	}

	ValidateTokenState::~ValidateTokenState()
	{
		//
	}
}
