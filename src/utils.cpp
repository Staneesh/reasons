#include "utils.hpp"

const Utils::Report Utils::load_entire_file(const char* path, unsigned char** where)
{
	FILE* file = fopen(path, "r");

    if (file == 0)
    {
        auto r = Utils::Report(Utils::Report::Type::ERROR, 
        "Unable to open file: "+(std::string(path)));
        
        return r;
    }

    fseek(file, 0, SEEK_END);
    unsigned fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    *where = (unsigned char *)calloc(1, fileSize + 1);
    memset(*where, 0, fileSize + 1);
    
    if (fread(*where, fileSize, 1, file) != 1) {
        return Utils::Report(Utils::Report::Type::ERROR, (char*)"Failed reading file!");
    }
    
    fclose(file);

	return Utils::Report();
}

std::string Utils::Report::disambiguate_message_type(const Type& t)
{
    if (t == Utils::Report::Type::ERROR)
    {
        return std::string("[ERROR]: ");
    }

    return std::string();
}

void Utils::Report::append(const Type& message_type, const std::string& add_to_report)
{
    std::string disambiguated_message_type = disambiguate_message_type(message_type);
    message += disambiguated_message_type + add_to_report + "\n";
}

void Utils::Report::append(const Report& r)
{
    message += r.message;
}

void Utils::Report::log()
{
    std::cout<<message<<std::endl;
}

void Utils::Report::log_if_bad()
{
    if (is_bad())
    {
        log();
    }
}

Utils::Report::Report(const Type& t, const std::string& m)
{
    append(t, m);
}

Utils::Report::Report(const Type& t, char* m)
{
    append(t, m);
}

Utils::Report::Report()
{
    message = "";
}

Utils::Report::~Report()
{
    //log_if_bad();
}

Utils::Report::Report(const Report& r)
{
    append(r);
}

bool Utils::Report::is_bad() const
{
    if (message.length() == 0)
    {
        return 0;
    }
    return 1;
}	

bool Utils::Report::is_good() const
{
    return !is_bad();
}

void Utils::Report::clear()
{
    message = "";
}

void Utils::TimedBlock::begin()
{
    timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    time_at_the_start = (u64)1e9 * current_time.tv_sec + current_time.tv_nsec;
    cycles_at_the_start = __rdtsc();
}

Utils::TimedBlock::TimedBlock(const std::string& name_pass)
{
    name = name_pass;
    begin();
}

void Utils::TimedBlock::end()
{
    timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    u64 this_time = (u64)1e9 * current_time.tv_sec + current_time.tv_nsec;
    u64 time_elapsed = this_time - time_at_the_start;
    u64 cycles_elapsed = __rdtsc() - cycles_at_the_start;

    std::cout<<"Block '"<<name<<"' says: \n\tTime elapsed = "<<time_elapsed<<" ms = "<<\
    (double)time_elapsed / 1e9<<" s\n\tCycles elapsed = "<<cycles_elapsed<<" C = "<<\
    (double)cycles_elapsed / 1e9<<" GC"<<std::endl;
}

Utils::TimedBlock::~TimedBlock()
{
    end();
}