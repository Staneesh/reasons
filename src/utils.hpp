#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <glm/glm.hpp>

#define LOG(x)       {std::cout<<"Line "<<__LINE__<<" in file "<<__FILE__<<" says: "<<#x<<" = "<<x<<std::endl;}
#define LOG_STRING(x){std::cout<<"Line "<<__LINE__<<" in file "<<__FILE__<<" says: "<<x<<std::endl;}
#define LOG_HERE     {std::cout<<"Line "<<__LINE__<<" in file "<<__FILE__<<" says: HERE!"<<std::endl;}
#define LOG_VEC(v)   {std::cout<<"Line "<<__LINE__<<" in file "<<__FILE__<<" says: "<<#v<<" = {"<<v.x<<", "<<v.y<<", "<<v.z<<"}"<<std::endl;}
#define LOG_VEC4(v)  {std::cout<<"Line "<<__LINE__<<" in file "<<__FILE__<<" says: "<<#v<<" = {"<<v.x<<", "<<v.y<<", "<<v.z<<", "<<v.w<<"}"<<std::endl;}
#define LOG_MAT4(m)  {std::cout<<"Line "<<__LINE__<<" in file "<<__FILE__<<" says: "<<#m<<" = {\n"<<m[0].x<<", "<<m[1].x<<", "<<m[2].x<<", "<<m[3].x<<std::endl; \
					  std::cout<<m[0].y<<", "<<m[1].y<<", "<<m[2].y<<", "<<m[3].y<<std::endl; \
					  std::cout<<m[0].z<<", "<<m[1].z<<", "<<m[2].z<<", "<<m[3].z<<std::endl; \
					  std::cout<<m[0].w<<", "<<m[1].w<<", "<<m[2].w<<", "<<m[3].w<<"\n}"<<std::endl;}

//NOTE(stanisz): disables the 'unused variable' warning 
#define UNUSED(x) (void)(x)

#define BEGIN_TIMED_BLOCK(block_name) Utils::TimedBlock timed_block_name##__LINE__((std::string)block_name);

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

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

		bool is_bad() const;
		bool is_good() const;

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
		Err_type log_if_err() const {if (type == Type::ERR) std::cout<<err<<std::endl;}

	private:
		Ok_type ok;
		Err_type err;
		Type type;
	};

	class TimedBlock
	{
		u64 cycles_at_the_start;
		//NOTE(stanisz): nanoseconds
		u64 time_at_the_start;
		std::string name;

		void begin();
		void end();
	public:
		TimedBlock(const std::string& name);
		~TimedBlock();
	};

	static const Report load_entire_file(const char* path, unsigned char** where);
	static u32 xorshift(u32* s);
};

#endif
