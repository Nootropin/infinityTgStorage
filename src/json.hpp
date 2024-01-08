#ifndef JSON_H
#define JSON_H
#include<boost/json.hpp>
#include<filesystem>
#include<fstream>
#include<iostream>
#include<string>
#include<tgbot/tgbot.h>
boost::json::object makeJsonFromFolder(std::string folderPath,std::string outputJsonFile,bool isRec);
void addFilesPaths(std::string filePath,std::string jsonFile,std::vector<std::string> array);
std::vector<std::string> getFilePathsFromJson(std::string filePath,std::string jsonFile);
boost::json::value getFolderFromJson(std::string filePath,std::string rootFolder,boost::json::value jsonFile);
template<typename T>
boost::json::array fromVectorToArray(std::vector<T>);
#endif