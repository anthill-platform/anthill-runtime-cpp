
#ifndef ONLINE_Social_Service_H
#define ONLINE_Social_Service_H

#pragma warning(disable : 4503)

#include "anthill/services/Service.h"
#include "anthill/requests/Request.h"
#include "anthill/ApplicationInfo.h"
#include <json/value.h>

#include <set>
#include <unordered_map>

namespace online
{
	typedef std::shared_ptr< class SocialService > SocialServicePtr;

	class AnthillRuntime;
    
	class SocialConnection
	{
		friend class SocialService;
        
    public:
    
        struct CredentialInfo
        {
            CredentialInfo(const Json::Value& data);
        
            Json::Value social;
        };
    
        typedef std::unordered_map<std::string, CredentialInfo> Credentials;
        
    public:
        SocialConnection(const Json::Value& data);
    
        const Credentials& getCredentials() const { return m_credentials; }
		const Json::Value& getProfile() const { return m_profile; }
        
    private:
        Credentials m_credentials;
        Json::Value m_profile;
	};
    
    typedef std::unordered_map<std::string, SocialConnection> SocialConnections;

	class SocialService : public Service
	{
		friend class AnthillRuntime;

	public:
		static const std::string ID;
        static const std::string API_VERSION;

		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request,
			const SocialConnections& connections) > GetConnectionsCallback;

	public:
		static SocialServicePtr Create(const std::string& location);
		virtual ~SocialService();

		void getConnections(
			const std::string& accessToken,
            const std::set<std::string>& profileFields,
            GetConnectionsCallback callback);
        
	protected:
		SocialService(const std::string& location);
        bool init();

	private:
	};
};

#endif
