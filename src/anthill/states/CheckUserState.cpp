
#include "anthill/states/CheckUserState.h"
#include "anthill/states/GenerateUserState.h"
#include "anthill/states/ValidateTokenState.h"
#include "anthill/states/AuthenticateState.h"
#include "anthill/states/StateMachine.h"
#include "anthill/Log.h"
#include "anthill/Utils.h"
#include "anthill/AnthillRuntime.h"

using namespace std::placeholders;

namespace online
{
	const std::string CheckUserState::StorageUsernameField = "online-username";
	const std::string CheckUserState::StoragePasswordField = "online-password";
	const std::string CheckUserState::StorageAccessTokeneField = "online-access-token";
    
    bool CheckUserState::s_doAuthenticate = true;

	CheckUserState::CheckUserState(std::shared_ptr<StateMachine> stateMachine) :
		AuthenticatedState(stateMachine)
	{
	}

	void CheckUserState::init()
	{
        if (!s_doAuthenticate)
        {
            complete();
            return;
        }
        
		LoginServicePtr ptr = AnthillRuntime::Instance().get<LoginService>();
		OnlineAssert((bool)ptr, "Login service is not initialized!");

		AnthillRuntime& online = AnthillRuntime::Instance();
		StoragePtr storage = online.getStorage();

		if (storage->has(StorageUsernameField) && storage->has(StoragePasswordField))
		{
			if (storage->has(StorageAccessTokeneField))
			{
				Log::get() << "Validating access token." << std::endl;

				std::string accessToken = storage->get(StorageAccessTokeneField);
                ptr->setCurrentAccessToken(accessToken);
				switchTo<ValidateTokenState>(accessToken);
			}
			else
			{
				authenticate();
			}
		}
		else
		{
			Log::get() << "No user found, generating new one." << std::endl;

			switchTo<GenerateUserState>();
		}
	}

	void CheckUserState::release()
	{
		//
	}

	CheckUserState::~CheckUserState()
	{
		//
	}
}
