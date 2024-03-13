#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <fstream>

namespace utils
{
    std::string getFileName(const std::string &fileName, const std::string &suffix)
    {
        size_t final_pos = fileName.find_last_of("/\\");
        size_t suffix_pos = fileName.find_last_of('.');
        return final_pos == std::string::npos ? "./output/" + fileName.substr(0, suffix_pos) + suffix : "./output/" + fileName.substr(final_pos + 1, suffix_pos - final_pos - 1) + suffix;
    }

    std::string parseText(const std::string &input)
    {
        // TODO: Your code here
        std::ifstream istrm(input);
        if (!istrm.is_open()) {
            std::cerr << "Failed to open file: " << input << std::endl;
            return "";
        }
        std::string content((std::istreambuf_iterator<char>(istrm)),(std::istreambuf_iterator<char>()));
        istrm.close();
        return content;
    }

    void output(const std::string &output, const std::string &data)
    {
        // TODO: Your code here
        std::ofstream ostrm(output);
        if (!ostrm.is_open()) {
            std::cerr << "Failed to create file: " << output << std::endl;
            return;
        }
        ostrm << data;
        ostrm.close();
    }

    std::string codingTable2String(const std::map<std::string, std::string> &codingTable)
    {
        std::string result = "";
        // TODO: Your code here
        for(std::pair<std::string,std::string> p : codingTable){
            result += p.first + ' ' + p.second + '\n';
        }
        return result;
    }

    void loadCodingTable(const std::string &input, std::map<std::string, std::string> &codingTable)
    {
        // TODO: Your code here
        std::string table = parseText(input);
        std::string data;
        std::string code;
        std::string::iterator blank,enterNext = table.begin(); //记录空格和换行符位置
        bool isBlank = false;
        for(std::string::iterator i = table.begin(); i != table.end(); i++){
            if(*i == ' ' && i - enterNext != 0){ //不是挨着的
                blank = i;
                isBlank = true;
            }
            if(*i == '\n' && i - blank != 1 && isBlank){
                data = table.substr(enterNext - table.begin(),blank - enterNext);
                code = table.substr(blank - table.begin() + 1,i - blank - 1);
                enterNext = i + 1;
                isBlank = false;
                codingTable.insert(std::make_pair(data,code));
            } 
        }
    }

    std::string compress(const std::map<std::string, std::string> &codingTable, const std::string &text)
    {
        std::string result;
        // TODO: Your code here
        std::string bitString = "";
        for(std::string::const_iterator it = text.begin(); it != text.end(); it++){
            std::string multiChar(1,*it);
            multiChar.push_back(*(it + 1));
            auto value = codingTable.find(multiChar);
            if(value != codingTable.end()){ //双字符查找成功
                bitString += value->second;
                it++;
            }
            else{ //单字符
                value = codingTable.find(std::string(1,*it));
                bitString += value->second;
            }
        }

        long length = bitString.length();

        while(bitString.length() % 8 != 0){
            bitString += '0';
        }

        for(int i = 0; i < 64; i++){
            result += (length & 1) ? '1' : '0';
            length >>= 1;
        }

        result += bitString;

        return result;
    }
};

#endif