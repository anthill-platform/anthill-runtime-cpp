
#include "anthill/services/ConfigService.h"
#include "anthill/requests/JsonRequest.h"
#include "anthill/AnthillRuntime.h"
#include "anthill/Utils.h"

#include <json/writer.h>

namespace online
{
    const std::string ConfigService::ID = "config";
    const std::string ConfigService::API_VERSION = "0.2";
    
    ConfigServicePtr ConfigService::Create(const std::string& location)
    {
        ConfigServicePtr _object(new ConfigService(location));
        if (!_object->init())
            return ConfigServicePtr(nullptr);
        
        return _object;
    }
    
    ConfigService::ConfigService(const std::string& location) :
        Service(location)
    {
        
	}
    
    void ConfigService::getConfig(GetConfigCallback callback)
    {
        const ApplicationInfo& applicationInfo = AnthillRuntime::Instance().getApplicationInfo();

		std::string url = getLocation() + "/config/" +
            applicationInfo.applicationName + "/" + applicationInfo.applicationVersion;
        JsonRequestPtr request = JsonRequest::Create(
            url,
            Request::METHOD_GET);
        
        if (request)
        {
            request->setAPIVersion(API_VERSION);
        
            request->setRequestArguments({
                {"gamespace", applicationInfo.gamespace}
            });
            
            request->setOnResponse([this, callback](const online::JsonRequest& request)
            {
               if (request.isSuccessful() && request.isResponseValueValid())
               {
				   const Json::Value& value = request.getResponseValue();
       
                   if (value.isMember("url"))
                   {
                       std::string url = value["url"].asString();
                       
                       JsonRequestPtr actualConfig = JsonRequest::Create(url, Request::METHOD_GET);
                       
                       if (actualConfig)
                       {
                           actualConfig->setParseAsJsonAnyway();
                           
                           actualConfig->setOnResponse([this, callback](const online::JsonRequest& actualConfig)
                           {
                               if (actualConfig.isSuccessful() && actualConfig.isResponseValueValid())
                               {
                                   const Json::Value& data = actualConfig.getResponseValue();
                                   callback(*this, actualConfig.getResult(), actualConfig, data);
                               }
                               else
                               {
                                   callback(*this, Request::INTERNAL_ERROR, actualConfig, Json::Value());
                               }
                           });
                       
                           actualConfig->start();
                       }
                       else
                       {
                           callback(*this, Request::INTERNAL_ERROR, request, Json::Value());
                       }
                   }
                   else
                   {
                        callback(*this, Request::INTERNAL_ERROR, request, Json::Value());
                   }
               }
               else
               {
				   callback(*this, request.getResult(), request, Json::Value());
               }
            });
        }
        else
        {
            OnlineAssert(false, "Failed to construct a request.");
        }
        
        request->start();
    }
    
    ConfigService::~ConfigService()
    {
        //
    }
    
    bool ConfigService::init()
    {
        return true;
    }
}
