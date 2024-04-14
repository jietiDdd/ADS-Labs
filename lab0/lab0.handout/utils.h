#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <fstream>
#include <vector>

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
        std::string fileEnding = output.substr(output.size() - 3);
        if(fileEnding == "idx"){
            std::ofstream ostrm(output);
            if (!ostrm.is_open()) {
                std::cerr << "Failed to create file: " << output << std::endl;
                return;
            }
            ostrm << data;
            ostrm.close();
        }
        else if(fileEnding == "zip"){
            std::string byteStream = "";
            uint64_t i = 0;
            char byte = 0;
            for(char c : data){
                i++;
                if(c == '1'){
                    byte += 1;
                }
                if(i == 8){
                    i = 0;
                    byteStream += byte;
                    byte = 0;
                    continue;
                }
                byte <<= 1;
            }
            std::ofstream ostrm(output,std::ios::binary);
            ostrm.write(byteStream.data(),byteStream.size());
            ostrm.close();
        }
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
        for(std::string::iterator i = table.begin(); i < table.end(); i++){
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
        for(std::string::const_iterator it = text.begin(); it < text.end(); it++){
            std::string multiChar(1,*it);
            if(it + 1 < text.end()){
                multiChar.push_back(*(it + 1));
                auto value = codingTable.find(multiChar);
                if(value != codingTable.end()){ //双字符查找成功
                    bitString += value->second;
                    it++;
                    continue;
                }
            }
            
            auto value = codingTable.find(std::string(1,*it));
            if(value != codingTable.end()){
                bitString += value->second;
            }    
        }

        uint64_t length = bitString.length();

        if (bitString.length() % 8 != 0) {
            uint64_t padding = 8 - (bitString.length() % 8);
            bitString += std::string(padding, '0');
        }

        std::string lengthByte = "";

        for(uint64_t i = 0; i < 64; i++){
            lengthByte = ((length & 1) ? '1' : '0') + lengthByte;
            length >>= 1;

            if((i + 1) % 8 == 0){
                result += lengthByte;
                lengthByte = "";
            }
        }

        result += bitString;

        return result;
    }
};

#endif