#include "utils.h"

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
        return Utils::Report(Utils::Report::Type::ERROR, "Failed reading file!");
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

void Utils::log_here()
{
    std::cout<<"HERE!"<<std::endl;
}