#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>

class Utils
{
public:
	class Report
	{
	public:
		enum class Type
		{
			NONE,
			ERROR,
			//NOTE(stanisz): more?
		};

		Report(const Type& t, const std::string& m)
		{
			append(t, m);
		}
		Report(const Type& t, char* m)
		{
			append(t, m);
		}
		Report()
		{
			message = "";
		}
		Report(const Report& r)
		{
			append(r);
		}

		const bool is_bad() const
		{
			if (message == "")
			{
				return 0;
			}
			return 1;
		}	
		const bool is_good() const
		{
			return !is_bad();
		}

		void append(const Type& message_type, const std::string& add_to_report);
		void append(const Report& r);

		void log();
		void log_if_bad();

	private:
		std::string message;
		std::string disambiguate_message_type(const Type& t);
	};

	static const Report load_entire_file(const char* path, unsigned char** where);
	static void log_here();	
};

#endif
