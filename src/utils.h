#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>

class Utils
{
public:
	class Report
	{
	public:
		//TODO(stanisz): static?
		enum class Type
		{
			ERROR,
			//NOTE(stanisz): more?
		};

		Report(const std::string& m)
		{
			message = m;
		}
		Report(char* m)
		{
			message = m;
		}
		Report()
		{
			message = "";
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

		Report append(const Type& message_type, const std::string& add_to_report);
	private:
		std::string message;
		std::string disambiguate_message_type(const Type& t);
	};

	static const Report load_entire_file(const char* path, unsigned char** where);
	
};

#endif
