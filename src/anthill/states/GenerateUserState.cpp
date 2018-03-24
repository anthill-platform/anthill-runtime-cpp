
#include "anthill/states/GenerateUserState.h"
#include "anthill/states/CheckUserState.h"
#include "anthill/states/StateMachine.h"

#include "anthill/Log.h"
#include "anthill/Utils.h"
#include "anthill/AnthillRuntime.h"

using namespace std::placeholders;

namespace online
{
	GenerateUserState::GenerateUserState(std::shared_ptr<StateMachine> stateMachine) :
		State(stateMachine)
	{
	}

	void GenerateUserState::init()
	{
		LoginServicePtr ptr = AnthillRuntime::Instance().get<LoginService>();
		OnlineAssert((bool)ptr, "Login service is not initialized!");

		std::string username = random_string(16) + "-" + random_string(16);
		std::string password = random_string(64);

		AnthillRuntime& online = AnthillRuntime::Instance();
		StoragePtr storage = online.getStorage();

		storage->set(Storage::StorageUsernameField, username);
		storage->set(Storage::StoragePasswordField, password);

		storage->save();

		Log::get() << "New user generated." << std::endl;

		online.getFutures().postNextUpdate([this] 
		{
			switchTo<CheckUserState>();
		});
	}

	void GenerateUserState::release()
	{
		//
	}

	GenerateUserState::~GenerateUserState()
	{
		//
	}
}
