
#ifndef _BOO_HTTP_CLIENT_MESSAGE_H_

#define _BOO_HTTP_CLIENT_MESSAGE_H_

#include <string>
#include "body.hpp"
#include "header.hpp"

namespace boohttp
{
	using namespace std;

	typedef enum {V_1_0, V_1_1, V_2_0, V_2TSL, V_AUTO, V_NON, V_UNSET} Version;

	class Message
	{
	public:
		Message();
		void withHeaderPair(string key, string value);
		void withHeader(const Header & header);
		void withHeader(const Header * header);
		void withBody(const Body & body);
		void withBody(const Body * body);
		void withBody(string body);
		void appendBody(string body);
		const Header * header() const;
		const Body * body() const;
		~Message();
	public:
		Version version = V_UNSET;

	protected: 
		Header _header;
		Body _body;
	};

}

#endif
