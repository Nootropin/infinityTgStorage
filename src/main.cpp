#include"bot.hpp"
#include"file.hpp"
#include"json.hpp"
namespace fs = std::filesystem;
void addFile(std::string filePath,cloudBot cBot,std::string jsonPlace,long long messageId)
{
    for(auto &x:fs::directory_iterator(filePath))
    {
        std::cout << x.path().string() << std::endl;
        if(!x.is_directory()) addFilesPaths(x.path().string(),jsonPlace,cBot.uploadFile(x.path().string(),messageId,"temperary"));
        else addFile(x.path().string(),cBot,jsonPlace,messageId);
    }
}
int main()
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
    // cBot.uploadFileOnServer("echobot",messageId);
    // makeJsonFromFolder(fs::current_path(),fs::current_path().string() + "/this.json");
    // addFile(fs::current_path(),cBot,"/media/chushpan/Main/TelegramInfinityCloudStorage/build/this.json",messageId);
    cBot.restoreFile("/media/chushpan/Main/TelegramInfinityCloudStorage/build/CMakeFiles/echobot.dir/src/json.cpp.o.d","this.json","someshit.c");
}