
#ifndef Online_Appliocation_Info_H
#define Online_Appliocation_Info_H

#include <set>
#include <string>

namespace online
{
	typedef std::set<std::string> AccessScopes;

	struct ApplicationInfo
	{
		std::string gamespace;
		std::string applicationName;
		std::string applicationVersion;
		AccessScopes requiredScopes;
        AccessScopes shouldHaveScopes;
        
        ApplicationInfo() :
            shouldHaveScopes({"*"})
        {
            
        }
	};
};

#endif
