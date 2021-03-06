#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::vector<std::string> &v, const std::string &sep):
                        keys(v), separator(sep) {}

ConfigParser::ConfigParser(const std::vector<std::string> &v):
                        ConfigParser(v, "=") {}

void ConfigParser::viewConfig(){
    std::cout << "separator:\t" << this->separator << std::endl;
    std::cout << "keys:\t";
    for(auto item : this->keys){
        std::cout << item << ", ";
    }
    std::cout << "\b\b " << std::endl;
}

std::unique_ptr<struct parsed_t> ConfigParser::parseString(std::string &str){

    std::string::size_type sep_pos;
    std::string key_c, key, val;

    sep_pos = str.find(this->separator);

    if(sep_pos == std::string::npos){
        std::cout << "ERROR: separator not found: '" << str << "'" << std::endl;
    }
    else{
        // std::cout << "found at: " << str.substr(sep_pos) << " " << sep_pos << std::endl;
        key = str.substr(0, sep_pos);
        val = str.substr(sep_pos + 1);

        if(this->trim(key)){
            std::cerr << "ERROR: trim(): '"<< key << "'" << std::endl;
        }

        if(this->trim(val)){
            std::cerr << "ERROR: trim(): '"<< val << "'" << std::endl;
        }
        // std::cout << "parsed:  '" << key << "'='" << val << "'" << std::endl;

        if(this->checkKeyValidity(key)){
            std::cerr << "ERROR: parsed key is not in key set inserted: '" << key << "'" << std::endl;
            return nullptr;
        }
        else{
            std::unique_ptr<struct parsed_t> p (new struct parsed_t(key, val));
            return p;
        }

    }

    return nullptr;
}

bool ConfigParser::parseStream(std::fstream &stream){

    std::map<std::string, std::string> m;
    std::map<std::string, std::string>::iterator it;
    std::string line;

    while ( getline (stream, line) ){
        // std::cout << "raw:" << line << '\n';
        auto p = this->parseString(line);
        if(p != nullptr){
            // std::cout << "par: " << p->lvalue << "=" << p->rvalue << std::endl;
            it = m.find(p->lvalue);
            if (it == m.end()){
                m.emplace(p->lvalue, p->rvalue);
            }
            else{
                m[p->lvalue] = p->rvalue;
            }
        }
    }

    this->output_map = m;
    if(this->checkMapValidity()){
        return false;
    }
    else{
        return true;
    }
}

bool ConfigParser::trim(std::string &str){

    if(str.length() <= 0){
        return true;
    }

    std::locale loc;

    while(std::isspace(str.back(),loc)){
        str.pop_back();
    }

    while(std::isspace(str.front(),loc)){
        str = str.substr(1);
    }

    return false;
}

bool ConfigParser::checkKeyValidity(const std::string &key){

    for(auto a: this->keys){
        // std::cout << "Cmp:" << a << ":" << key << std::endl;
        if(!key.compare(a)){
            return false;
        }
        else{
            continue;
        }
    }

    return 1;
}

bool ConfigParser::checkMapValidity(){
    std::cerr << "ERROR: Override ConfigParser::checkMapValidity() virtual method!" << std::endl;
    return false;
}

std::map<std::string, std::string> ConfigParser::getMap(){
    return this->output_map;
}