
#ifndef ONLINE_Login_Service_H
#define ONLINE_Login_Service_H

#include "Service.h"
#include "../requests/Request.h"
#include "../ApplicationInfo.h"
#include <json/value.h>

#include <set>
#include <unordered_map>

namespace online
{
	typedef std::shared_ptr< class LoginService > LoginServicePtr;

	class AnthillRuntime;

	class LoginService : public Service
	{
		friend class AnthillRuntime;

	public:
		struct MergeOption
		{
			std::string m_name;
			std::string m_credential;
			std::string m_account;
			Json::Value m_profile;
		};

	public:
		typedef std::set<std::string> Scopes;
		typedef std::set<std::string> Credentials;
		typedef std::vector<MergeOption> MergeOptions;

		typedef std::function< void(const LoginService& service, Request::Result result, const Request& request,
			const std::string& accessToken, const std::string& credential, const std::string& account, const Scopes& scopes) > AuthenticationCallback;
        
		typedef std::function< void(const LoginService& service, Request::Result result, const Request& request,
            const Credentials& credentials) > GetCredentialsCallback;
            
		typedef std::function< void(const MergeOption& selectedOption) > MergeResolveCallback;
		typedef std::function< void(const LoginService& service, const MergeOptions options, MergeResolveCallback resolve) > MergeRequiredCallback;

		typedef std::function< void(const LoginService& service, Request::Result result, const Request& request,
            const std::string& credential, const std::string& account, const Scopes& scopes) > ValidationCallback;

	public:
		static const std::string ID;
		static const std::string Anonymous;
        static const std::string API_VERSION;

	public:
		static LoginServicePtr Create(const std::string& location);
		virtual ~LoginService();

		void extend(
			const std::string& accessToken,
			const std::string& extendWith,
			const Scopes& scopes,
			AuthenticationCallback callback);
        
		void getCredentials(const std::string& accessToken, GetCredentialsCallback callback);
		void getCredentials(GetCredentialsCallback callback);

		void authenticate(
			const std::string& credentialType,
			const std::string& gamespace,
			const Scopes& scopes,
			const Request::Fields& other,
			AuthenticationCallback callback,
			MergeRequiredCallback mergeRequiredCallback,
            const Scopes& shouldHaveScopes = {"*"});

		void attach(
			const std::string& credentialType,
			const std::string& gamespace,
			const Scopes& scopes,
			const Request::Fields& other,
			AuthenticationCallback callback,
			MergeRequiredCallback mergeRequiredCallback,
            const Scopes& shouldHaveScopes = {"*"});
		
		void resolve(
			const std::string& resolveToken,
			const std::string& methodToResolve,
			const std::string& resolveWith,
			const Scopes& scopes,
			const Request::Fields& other,
			AuthenticationCallback callback,
            const Scopes& shouldHaveScopes = {"*"});

		void authenticateAnonymous(
			const std::string& username,
			const std::string& password,
			const std::string& gamespace,
			const Scopes& scopes,
			const Request::Fields& other,
			AuthenticationCallback callback,
			MergeRequiredCallback mergeRequiredCallback,
            const Scopes& shouldHaveScopes = {"*"});

		void validateAccessToken(ValidationCallback callback);
		void validateAccessToken(const std::string& accessToken, ValidationCallback callback);

		const std::string& getCurrentAccessToken() const;
		void setCurrentAccessToken(const std::string& token);

	protected:
		LoginService(const std::string& location);
		bool init();

	private:
		std::string m_currentToken;
	};
};

#endif
