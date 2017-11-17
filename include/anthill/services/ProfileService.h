
#ifndef ONLINE_Profile_Service_H
#define ONLINE_Profile_Service_H

#include "Service.h"
#include "../requests/Request.h"
#include "../ApplicationInfo.h"
#include <json/value.h>

#include <set>
#include <unordered_map>

namespace online
{
    typedef std::shared_ptr< class ProfileService > ProfileServicePtr;
    
    class AnthillRuntime;

	class UserProfile
	{
		friend class ProfileService;

	protected:
		virtual void read(const Json::Value& profile) = 0;
		virtual void write(Json::Value& profile) const = 0;
	};
    
    class ProfileService : public Service
    {
        friend class AnthillRuntime;
	public:
		typedef std::unordered_map<std::string, Json::Value> Profiles;

		typedef std::function< void(const ProfileService& service, Request::Result result, const Request& request, 
			const Profiles& profiles ) > GetMassUserProfilesCallback;

		typedef std::function< void(const ProfileService& service, Request::Result result, const Request& request) > GetUserProfileCallback;
		typedef std::function< void(const ProfileService& service, Request::Result result, const Request& request) > SetUserProfileCallback;

        typedef std::function< void(const ProfileService& service, Request::Result result, const Request& request,
			const Json::Value& profile) > GetProfileCallback;
        
        typedef std::function< void(const ProfileService& service, Request::Result result, const Request& request,
			const Json::Value& profile) > SetProfileCallback;
        
    public:
        static const std::string ID;
        static const std::string API_VERSION;
        
    public:
        static ProfileServicePtr Create(const std::string& location);
        virtual ~ProfileService();
        
        void getMassProfiles(const std::set<std::string>& accounts, const std::set<std::string>& profileFields, 
			const std::string& accessToken, GetMassUserProfilesCallback callback);

        void getProfile(const std::string& account, const std::string& path, const std::string& accessToken, GetProfileCallback callback);
		void setProfile(const std::string& account, const std::string& path, const Json::Value& profile, bool merge,
			const std::string& accessToken, SetProfileCallback callback);

		void getProfile(const std::string& account, const std::string& path, UserProfile& userProfile, const std::string& accessToken, GetUserProfileCallback callback);
		void setProfile(const std::string& account, const std::string& path, const UserProfile& userProfile, bool merge,
			const std::string& accessToken, SetUserProfileCallback callback);
        
        void getMyProfile(const std::string& path, const std::string& accessToken, GetProfileCallback callback);
		void setMyProfile(const std::string& path, const Json::Value& profile, bool merge, const std::string& accessToken, SetProfileCallback callback);

		void getMyProfile(const std::string& path, UserProfile& profile, const std::string& accessToken, GetUserProfileCallback callback);
		void setMyProfile(const std::string& path, const UserProfile& profile, bool merge, const std::string& accessToken, SetUserProfileCallback callback);

    protected:
        ProfileService(const std::string& location);
        bool init();
        
    private:
    };
};

#endif
