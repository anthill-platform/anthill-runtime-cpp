
#ifndef ONLINE_JsonRequests_H
#define ONLINE_JsonRequests_H

#include "Request.h"

#include <json/value.h>
#include <functional>

namespace online
{
	typedef std::shared_ptr< class JsonRequest > JsonRequestPtr;

	class AnthillRuntime;

	class JsonRequest: public StringStreamRequest
	{
		friend class AnthillRuntime;

	public:
		typedef std::function< void(const JsonRequest&) > ResponseCallback;

	public:
		static JsonRequestPtr Create(const std::string& location, Request::Method method);
		virtual ~JsonRequest();

		bool isResponseValueValid() const;
		const Json::Value& getResponseValue() const;
        void setParseAsJsonAnyway() { m_parseAsJsonAnyway = true; }

		void setOnResponse(ResponseCallback onResponse);

	protected:
		JsonRequest(const std::string& location, Request::Method method);
		virtual bool init() override;

		// called once the request is done
		virtual void done() override;

	private:
		bool m_responseValueValid;
        bool m_parseAsJsonAnyway;
		Json::Value	m_responseValue;
		ResponseCallback m_onResponse;
	};
};

#endif
