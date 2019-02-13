#pragma once

#ifndef _BOO_HTTP_CLIENT_HEADER_H
#define  _BOO_HTTP_CLIENT_HEADER_H

#include <string>
#include <map>
#include <vector>

namespace boohttp
{
	using namespace std;

	class Container
	{
	public:
		Container();
		void merge(const Container * header);
		void set(string key, string value);
		void unset(string key);
		string value(string key);
		bool has(string key);
		void empty();
		const map<string, string> & all() const;

		static string combine(vector<string> value);
		~Container();
	private:
		map<string, string> items;
	};
}

#endif
