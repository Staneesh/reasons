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

	template<typename T>
	class Option
	{
	public:
		enum class Type
		{
			NONE,
			SOME
		};

		Option() {type = Type::NONE;}
		Option(T val) {value = val; type = Type::SOME;}
		bool is_some() const {if(type == Type::SOME) return true; assert(type == Type::NONE); return false;}
		bool is_none() const {return !is_some();}
		T get_value() const {assert(type == Type::SOME); return value;}

	private:
		T value;
		Type type;
	};

	template<typename Ok_type, typename Err_type>
	class Result
	{
	public:
		enum class Type
		{
			OK,
			ERR
		};

		Result(){std::cout<<"Could not create an empty result."<<std::endl; assert(1 == 0);}
		Result(Ok_type val) {ok = val; type = Type::OK;}
		Result(Err_type val) {err = val; type = Type::ERR;}
		bool is_ok() const {if (type == Type::OK) return true; assert(type == Type::ERR); return false;}
		bool is_err() const {return !is_ok();}
		Ok_type get_ok() const {assert(type == Type::OK); return ok;}
		Err_type get_err() const {assert(type == Type::ERR); return err;}

	private:
		Ok_type ok;
		Err_type err;
		Type type;
	};

	static const Report load_entire_file(const char* path, unsigned char** where);
	static void log_here();	
	static void log_vec3(const glm::vec3& v);
};

#endif
