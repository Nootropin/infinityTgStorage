#include"bot.hpp"
#include"file.hpp"
#include"json.hpp"
#include<random>
#include<chrono>
namespace fs = std::filesystem;
boost::json::object sortJson(boost::json::value jsonValue);
std::string generateString(int length)
{
    srand(std::chrono::system_clock::now().time_since_epoch().count());
    std::string retString = "";
    for(int i = 0;i<length;i++)
    {
        retString += char(65 + random()%25);
    }
    return retString;
}
void listJsonOfPaths(boost::json::value &jsonValue,int intendLength)
{
    for(auto &key:jsonValue.get_object())
    {
        if(key.value().at("type") == "folder") 
        {
            std::string intend = "";
            intend.append(intendLength,' ');
            intend+="|---";
            boost::json::value json = sortJson(key.value().at("filesJson"));
            std::cout << intend << key.key() << std::endl;
            listJsonOfPaths(json,intendLength + 4);
        }
        else
        {
            std::string intend = "";
            intend.append(intendLength,' ');
            intend+="|---";
            size_t value = 0;
            if(key.value().at("fileIds").is_array()) value = key.value().at("fileIds").as_array().size();
            std::cout << intend << key.key() << " Size ~=" <<  value * 20 << "MB"  << std::endl;
        }
    }
}
boost::json::object sortJson(boost::json::value jsonValue)
{
    std::vector<boost::json::object::value_type> folders;
    std::vector<boost::json::object::value_type> files;
    boost::json::object returnValue;
    for(auto &key:jsonValue.get_object())
    {
        if(key.value().at("type") == "folder") 
        {
            folders.push_back(key);
        }
        else
        {
            files.push_back(key);
        }
    }
    for(size_t i = 0;i<folders.size();i++)
    {
        returnValue[folders.at(i).key()] =  folders.at(i).value();
        returnValue[folders.at(i).key()] = folders.at(i).value() ;
    }
    for(size_t i = 0;i<files.size();i++)
    {
        returnValue[files.at(i).key()] =  files.at(i).value();
        returnValue[files.at(i).key()] = files.at(i).value() ;
    }
    return returnValue;
}
void addFiles(std::string filePath,cloudBot cBot,std::string jsonPlace,long long messageId)
{
    std::string tempFolderName = generateString(10);
    fs::path tempPath = fs::path(fs::absolute(fs::current_path()).string() + "/" + tempFolderName);
    if(fs::exists(filePath) && fs::is_directory(fs::path(filePath)))
    {
        for(auto &x:fs::directory_iterator(filePath))
        {
            fs::create_directory(tempPath);
            std::cout << x.path().string() << std::endl;
            if(!x.is_directory())
            { 
                std::vector<std::string> arr = cBot.uploadFile(x.path().string(),messageId,tempFolderName);
                std::cout << arr.size() << "- files amount" << std::endl;
                for(auto &y:arr)
                {
                    std::cout << y << std::endl;
                }
                addFilesPaths(x.path().string(),jsonPlace,arr);
            }
            else if(x.is_directory()) addFiles(x.path().string(),cBot,jsonPlace,messageId);
            std::cout << fs::absolute(tempPath) << std::endl;
            std::cout << fs::remove_all(fs::absolute(tempPath)) << std::endl;
            std::cout << fs::remove(fs::absolute(tempPath)) << std::endl;
        }
    }
}
int main(int argc,char* argv[])
{
    std::string token = "";
    long long messageId = 0;
    std::string envFile = ".env";
    std::string inputId;
    std::ifstream file(envFile);
    getline(file,token);
    getline(file,inputId);
    messageId = std::stoll(inputId);
    TgBot::Bot bot(token);
    cloudBot cBot(bot);
    //makeJsonFromFolder("/media/chushpan/Main/TelegramInfinityCloudStorage/build","this.json");
    std::cout << argv[1] << std::endl;
    if(argc >= 3)
    {
        char checkChar = argv[1][0];
        if(checkChar == 'u')
        {
            std::cout << argv[2] << std::endl;
            if(fs::exists(argv[2]))
            {
                std::string path = fs::absolute(fs::path(argv[2]));
                std::vector<std::string> arr = cBot.uploadFile(path,messageId,"temp");
                for(auto &x:arr)
                {
                    std::cout << x << std::endl;
                }
                addFilesPaths(path,"this.json",arr);
            }
        }
        else if(checkChar == 'd')
        {
            if(!fs::exists(argv[2]))
            {
                std::string path = fs::absolute(fs::path(argv[2]));
                std::cout << path << std::endl;
                fs::create_directories(path + "temp");
                cBot.downloadFilesFromCloud(path,"this.json",path +"temp/temp");
                connectSplittedFiles(path +"temp/temp",10000,argv[2]);
                fs::remove_all(path +"temp");
                std::cout << "File downloaded" << std::endl;
            }
        }   
        else if(checkChar == 'f')
        {
            std::ifstream f("this.json");
            boost::json::value json = boost::json::parse(f);
            std::cout << "Json parsed" << std::endl;
            std::string str = fs::current_path().string();
            std::string filePath = argv[2];
            boost::json::value jsonValue;
            std::cout << filePath.substr(str.size() + 1) << std::endl;
            jsonValue = getFolderFromJson(filePath,fs::current_path().string(),json);
            std::cout << boost::json::serialize(jsonValue);
    }
    }
    else if(argc >= 2)
    {
        char checkChar = argv[1][0];
        if(checkChar == 'i')
        {
            makeJsonFromFolder(fs::current_path(),"this.json",0);
            addFiles(fs::current_path(),cBot, fs::current_path().string() + "/this.json",messageId);
        }
        if(checkChar == 'l')
        {
            std::fstream file("this.json");
            boost::json::value jsonValue = boost::json::parse(file);
            boost::json::value sorted = sortJson(jsonValue);
            listJsonOfPaths(sorted,0);
        }
    }
}