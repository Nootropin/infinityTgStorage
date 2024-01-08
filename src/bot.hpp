#ifndef BOT_H
#define BOT_H
#include<tgbot/tgbot.h>
class cloudBot
{
    public:
        cloudBot(TgBot::Bot& bot);
        void downloadFile(std::string filePath,std::string outputName);
        std::vector<std::string> uploadFile(std::string fileName,long long messageId,std::string tempFolderName);
        void downloadFilesFromCloud(std::string filePath,std::string jsonFile,std::string outputPath);
        void restoreFile(std::string fileName,std::string jsonFile,std::string outputFileName,std::string tempFolderName);
        void downloadFolder(std::string folderName,std::string jsonFile,std::string outputFolderName);
    private:
        std::string uploadFileOnServer(std::string filename,long long messageId);
        TgBot::Bot* tgBot;
};
#endif