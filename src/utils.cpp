#include "utils.h"

const Utils::Report Utils::load_entire_file(const char* path, unsigned char** where)
{
	FILE* file = fopen(path, "r");
    
    fseek(file, 0, SEEK_END);
    unsigned fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    *where = (unsigned char *)calloc(1, fileSize + 1);
    memset(*where, 0, fileSize + 1);
    
    if (fread(*where, fileSize, 1, file) != 1) {
        return Utils::Report("Failed reading file!");
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

Utils::Report Utils::Report::append(const Type& message_type, const std::string& add_to_report)
{
    std::string disambiguated_message_type = disambiguate_message_type(message_type);
    return Utils::Report(message + add_to_report);
}