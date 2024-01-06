#include"json.hpp"
namespace fs = std::filesystem;
template<typename T>
boost::json::array fromVectorToArray(std::vector<T> arr) // transfer std::vector to boost::json::array 
{
    boost::json::array jsonArray;
    for (const auto &str : arr) {
        jsonArray.emplace_back(str);
    }
    return jsonArray;
}
boost::json::object makeJsonFromFolder(std::string folderPath,std::string outputJsonFile) // creating a json out from folder
{
    std::ofstream f(outputJsonFile);
    boost::json::object value;
    for(auto i:fs::directory_iterator(folderPath))
    {
        if(i.is_directory())
        {
            value[std::string(i.path())] = {{"type","folder"},{"filesJson",makeJsonFromFolder(i.path(),"")}};
        }
        else
        {
            value[std::string(i.path())] = {{"type","file"},{"fileIds", 0}}; 
        }
    }
    f << boost::json::serialize(value);
    return value;
}
void addFilesPaths(std::string filePath,std::string jsonFile,std::vector<std::string> array) // changing folder json by adding file paths
{
    std::ifstream f(jsonFile);
    boost::json::object json = boost::json::parse(f).as_object();
    std::string writeString = "";
    json[filePath] = {{"type","file"},{"fileIds",fromVectorToArray(array)}};
    for(auto &x: array)
    {
        writeString += x + ",";
    }
    std::string jsonWrite = boost::json::serialize(json);
    f.close();
    std::ofstream fileWriteTo(jsonFile);
    fileWriteTo << jsonWrite;
    fileWriteTo.close();
}
std::vector<std::string> getFilePathsFromJson(std::string filePath,std::string jsonFile) // parsing file paths from file 
{
    std::ifstream f(jsonFile);
    boost::json::value json = boost::json::parse(f);
    boost::json::value jsonArray = json.at(filePath).at("fileIds");
    std::vector<std::string> arr;
    for(auto const& element : jsonArray.as_array())
    {
        arr.push_back(element.as_string().c_str());
    }
    f.close();
    return arr;
}