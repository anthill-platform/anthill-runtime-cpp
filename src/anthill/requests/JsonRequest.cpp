
#include "anthill/requests/JsonRequest.h"
#include <json/reader.h>

namespace online
{
	JsonRequestPtr JsonRequest::Create(const std::string& location, Request::Method method)
	{
		JsonRequestPtr _object(new JsonRequest(location, method));
		if( !_object->init() )				
			return JsonRequestPtr(nullptr);

		return _object;
	}
	
	JsonRequest::JsonRequest(const std::string& location, Request::Method method) :
		StringStreamRequest(location, method),
		m_responseValueValid(false),
        m_parseAsJsonAnyway(false)
	{
		//
	}

	bool JsonRequest::isResponseValueValid() const
	{
		return m_responseValueValid;
	}

	const Json::Value& JsonRequest::getResponseValue() const
	{
		return m_responseValue;
	}

	void JsonRequest::setOnResponse(JsonRequest::ResponseCallback onResponse)
	{
		m_onResponse = onResponse;
	}

	void JsonRequest::done()
	{
		Request::done();

		if (m_parseAsJsonAnyway || getResponseContentType() == "application/json")
		{
			m_responseValueValid = Json::Reader().parse(getResponseAsString(), m_responseValue);
		}

		if (m_onResponse)
		{
			m_onResponse(*this);
		}
	}

	JsonRequest::~JsonRequest()
	{
		//
	}

	bool JsonRequest::init()
	{
		return Request::init();
	}
}
