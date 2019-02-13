
#ifndef _BOO_HTTP_CLIENT_REQUEST_H_
#define _BOO_HTTP_CLIENT_REQUEST_H_

#include "message.hpp"
#include "body.hpp"
#include "url.hpp"
#include "header.hpp"
#include "postdata.hpp"

namespace boohttp
{
	typedef enum {
		AUTH_BASIC,
		AUTH_DIGEST,
		AUTH_DIGEST_IE,
		// AUTH_BEARER,
		AUTH_NEGOTIATE,
		AUTH_NTLM,
		AUTH_NTLM_WB,
		AUTH_ANY,
		AUTH_ANYSAFE,
		AUTH_ONLY,
		AUTH_NON
	} AuthType;
	class Auth
	{
	public:
		string user;
		string passwd;
		AuthType type;
	};

	typedef enum {
		POST,
		GET,
		PUT,
		DEL,
		OPTIONS
	} Method;

	class Request : public Message
	{
	public:
		Request() {}
		Request(string url);
		Request(string url, Method method);
		Request(Url url);
		Request(Url url, Method method);
		void withAuth(string user, string pass, AuthType type);
		void withAuth(Auth auth);
		void withPostData(const PostData * data);
		~Request() {};
	public:
		Method method = GET;
		PostData postData;
		string referer;
		Auth auth = Auth{"", "", AUTH_NON};
		Url url;
	};
}

#endif
