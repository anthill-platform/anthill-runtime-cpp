
#ifndef ONLINE_Social_Service_H
#define ONLINE_Social_Service_H

#pragma warning(disable : 4503)

#include "anthill/services/Service.h"
#include "anthill/requests/Request.h"
#include "anthill/ApplicationInfo.h"
#include <json/value.h>

#include <set>
#include <unordered_map>
#include <list>

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
	    
	class SocialRequest
	{
		
		enum class Type
		{
			group = 1,
			account,
			unknown
		};

		friend class SocialService;
        
    public:
    
    public:
        SocialRequest(const Json::Value& data);
    
        Type getType() const { return m_type; }
		const std::string& getObject() const { return m_object; }
		const Json::Value& getPayload() const { return m_payload; }
		const std::string& getKey() const { return m_key; }
        
    private:
        Type m_type;
		std::string m_object;
		Json::Value m_payload;
		std::string m_key;
	};
    
    typedef std::unordered_map<std::string, SocialConnection> SocialConnections;
	
    typedef std::list<SocialRequest> SocialRequests;

	class SocialService : public Service
	{
		friend class AnthillRuntime;

	public:
		static const std::string ID;
        static const std::string API_VERSION;
		
		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request,
			const SocialConnections& connections) > GetConnectionsCallback;
		
		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request,
			const std::string& key) > AddConnectionsCallback;
		
		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request,
			const SocialRequests& requests) > GetRequestsCallback;

		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request) > DeleteConnectionsCallback;
		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request) > ApproveConnectionsCallback;
		typedef std::function< void(const SocialService& service, Request::Result result, const Request& request) > RejectConnectionsCallback;
		
	public:
		static SocialServicePtr Create(const std::string& location);
		virtual ~SocialService();

		void getConnections(
			const std::string& accessToken,
            const std::set<std::string>& profileFields,
            GetConnectionsCallback callback);

		void getIncomingRequests(
			const std::string& accessToken,
            GetRequestsCallback callback);

		void getOutgoingRequests(
			const std::string& accessToken,
            GetRequestsCallback callback);
        
		void addConnection(
			const std::string& account,
			const std::string& accessToken,
            AddConnectionsCallback callback,
			bool approval = true,
			const Json::Value& notify = Json::Value(),
			const Json::Value& payload = Json::Value());
		
		void deleteConnection(
			const std::string& account,
			const std::string& accessToken,
            DeleteConnectionsCallback callback,
			const Json::Value& notify = Json::Value());

		void approveConnection(
			const std::string& account,
			const std::string& key,
			const std::string& accessToken,
            ApproveConnectionsCallback callback,
			const Json::Value& notify = Json::Value());

		void rejectConnection(
			const std::string& account,
			const std::string& key,
			const std::string& accessToken,
            RejectConnectionsCallback callback,
			const Json::Value& notify = Json::Value());

	protected:
		SocialService(const std::string& location);
        bool init();

	private:
	};
};

#endif
