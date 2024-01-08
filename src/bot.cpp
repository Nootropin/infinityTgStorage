#include"bot.hpp"
#include"file.hpp"
#include"json.hpp"
namespace fs = std::filesystem;
cloudBot::cloudBot(TgBot::Bot& bot) // constructor of bot
{
    this->tgBot = &bot;
}
std::vector<std::string> cloudBot::uploadFile(std::string fileName,long long messageId,std::string tempFolderName) // split file and upload it on telegram server
{
    std::vector<std::string> returnArray; // array of filePath
    std::string path = tempFolderName + "/tempFile";
    splitFilesIntoParts(fileName,19922944,19922944,path); // 19922944 - 19 Mb
    for(int i = 0;fs::exists(path + std::to_string(i));i++)
    {
        std::string str = this->uploadFileOnServer(path + std::to_string(i),messageId);
        returnArray.push_back(str);
    }
    return returnArray; // returning all pathes to splitted files
}
std::string cloudBot::uploadFileOnServer(std::string filename,long long messageId) // upload single file to telegram server
{
    TgBot::InputFile::Ptr file = TgBot::InputFile::fromFile(filename.c_str(),"text/plain");
    TgBot::Message::Ptr mess = this->tgBot->getApi().sendDocument(messageId,file);
    return mess->document->fileId;
}
void cloudBot::downloadFile(std::string filePath,std::string outputName) // download file from telegram server to local 
{
    TgBot::File::Ptr file = this->tgBot->getApi().getFile(filePath);
    std::string filePathId = file->filePath;
    std::string fileData = this->tgBot->getApi().downloadFile(filePathId);
    std::ofstream f(outputName);
    f << fileData;
    f.close();
}
void cloudBot::restoreFile(std::string fileName,std::string jsonFile,std::string outputFileName,std::string tempFolder) // get splitted files and connect them
{
    std::filesystem::create_directory(tempFolder);
    this->downloadFilesFromCloud(fileName,jsonFile,tempFolder + "/" + outputFileName);
    connectSplittedFiles(tempFolder + "/" + outputFileName,19922944,outputFileName); // 19922944 - 19 MB
    fs::remove_all(tempFolder); // remove temparary folder
}
void cloudBot::downloadFilesFromCloud(std::string filePath,std::string jsonFile,std::string outputPath) // get filePathes and download
{
    std::vector<std::string> paths = getFilePathsFromJson(filePath,jsonFile);
    if(paths.size() > 0)
    {
        if(paths[0] != "folder")
        {
            for(int i = 0;i<paths.size();i++)
            {
                std::cout << "Downloading " << paths[i] << std::endl;
                downloadFile(paths[i],outputPath  + std::to_string(i));
            }
        }
    }
}
void cloudBot::downloadFolder(std::string folderName,std::string jsonFile,std::string outputFolderName)
{
    
}