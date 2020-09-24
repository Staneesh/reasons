#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

class Utils
{
public:
	class Report
	{
	public:
		enum class Type
		{
			ERROR,
			//NOTE(stanisz): more?
		};

		Report();
		Report(const Type& t, const std::string& m);
		Report(const Type& t, char* m);
		Report(const Report& r);
		~Report();

		const bool is_bad() const;
		const bool is_good() const;

		void append(const Type& message_type, const std::string& add_to_report);
		void append(const Report& r);

		void log();
		void log_if_bad();

		void clear();

	private:
		std::string message;
		std::string disambiguate_message_type(const Type& t);
	};

	static const Report load_entire_file(const char* path, unsigned char** where);
	static void log_here();	
	static void log_vec3(const glm::vec3& v);
};

#endif
