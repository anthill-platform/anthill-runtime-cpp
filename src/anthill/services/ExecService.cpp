
#include "anthill/services/ExecService.h"
#include "anthill/requests/JsonRequest.h"
#include "anthill/AnthillRuntime.h"
#include "anthill/Utils.h"
#include "anthill/Websockets.h"

#include <json/writer.h>

namespace online
{
	const std::string ExecService::ID = "exec";
    const std::string ExecService::API_VERSION = "0.2";
    
    
    ExecSession::ExecSession(const std::string& location) :
        m_sockets(WebsocketRPC::Create()),
        m_location(location)
    {
    }

    void ExecSession::callMain(const Json::Value& arguments, FunctionSuccessCallback success, FunctionFailCallback failture, float timeout)
    {
        call("main", arguments, success, failture, timeout);
    }
    
    void ExecSession::call(const std::string& method, const Json::Value& arguments, FunctionSuccessCallback success, FunctionFailCallback failture, float timeout)
    {
        Json::Value args(Json::ValueType::objectValue);
        
        args["method_name"] = method;
        args["arguments"] = arguments;

		if (!m_sockets->isConnected())
		{
			failture(599, "Socket is closed", "");
			return;
		}
    
        m_sockets->request("call", success, failture, args, timeout);
    }
    
    bool ExecSession::isActive() const
    {
        return m_sockets->isConnected();
    }
    
    void ExecSession::connect(const std::string& accessToken, const std::string& className, const Json::Value& args,
        ExecSession::SessionCreatedCallback onCreated, ExecSession::SessionClosedCallback onClosed)
    {
        const ApplicationInfo& applicationInfo = AnthillRuntime::Instance().getApplicationInfo();

        WebsocketRPC::Options options = {
            {"access_token", accessToken},
            {"args", Json::FastWriter().write(args)},
        };
        
        std::string path = m_location + "/session/" + applicationInfo.applicationName + "/" +
            applicationInfo.applicationVersion + "/" + className;
        
        std::map<std::string, std::string> extraHeaders = {
            {"X-Api-Version", ExecService::API_VERSION}
        };
        
        m_sockets->connect(path, options,
            [=](bool success, int code)
        {
            if (success)
            {
                onCreated(*this, Request::SUCCESS);
            }
            else
            {
                onCreated(*this, (Request::Result)code);
            }
        }, onClosed, extraHeaders);
    }
    
    void ExecSession::update()
    {
        m_sockets->update();
    }
    
	void ExecSession::waitForShutdown()
	{
		m_sockets->waitForShutdown();
	}

    void ExecSession::disconnect(int code, const std::string& reason)
    {
        m_sockets->disconnect(code, reason);
    }
    
    ExecSessionPtr ExecSession::Create(const std::string& location)
    {
        return std::make_shared<ExecSession>(location);
    }

    //////////////////////

	ExecServicePtr ExecService::Create(const std::string& location)
	{
		ExecServicePtr _object(new ExecService(location));
		if (!_object->init())
			return ExecServicePtr(nullptr);

		return _object;
	}

	ExecService::ExecService(const std::string& location) :
		Service(location)
	{
		
    }
    
    void ExecService::callMain(const std::string& accessToken, const Json::Value& arguments, FunctionCallCallback callback)
    {
        call(accessToken, "main", arguments, callback);
    }
    
    void ExecService::call(const std::string& accessToken, const std::string& method, const Json::Value& arguments, FunctionCallCallback callback)
    {
        OnlineAssert(arguments.isArray() || arguments.isObject(), "Arguments should be an array or an object.");
    
        const ApplicationInfo& applicationInfo = AnthillRuntime::Instance().getApplicationInfo();

        JsonRequestPtr request = JsonRequest::Create(
			getLocation() + "/call/" + applicationInfo.applicationName + "/" + applicationInfo.applicationVersion + "/" + method, Request::METHOD_POST);
        
		if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            Json::FastWriter fastWriter;
            
            request->setPostFields({
                {"access_token", accessToken },
                {"args", fastWriter.write(arguments) }
            });
            
			request->setOnResponse([=](const online::JsonRequest& request)
			{
                const Json::Value& value = request.getResponseValue();
                callback(*this, request.getResult(), request, value);
			});
		}
		else
		{
			OnlineAssert(false, "Failed to construct a request.");
		}

		request->start();
    }
    
    ExecSessionPtr ExecService::session()
    {
        return ExecSession::Create(getLocation());
    }
    
	ExecService::~ExecService()
	{
		//
	}

	bool ExecService::init()
	{
		return true;
	}
}
