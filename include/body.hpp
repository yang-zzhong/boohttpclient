#pragma once

#ifndef _BOO_HTTP_CLIENT_H_
#define _BOO_HTTP_CLIENT_H_

#include <string>

namespace boohttp
{
	using namespace std;

	class Body
	{
	public:
		Body();
		Body(string body);
		void append(string body);
		const string str() const;
		~Body();
	private:
		string body;
	};
}

#endif
