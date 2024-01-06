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
    std::cout << "File name is " << fileName << std::endl;
    fs::remove_all(tempFolderName);
    fs::create_directory(tempFolderName); // creating empty folder
    fs::current_path(fileName.substr(0,fileName.find_last_of('/'))); // setting current path at path of file
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
void cloudBot::restoreFile(std::string fileName,std::string jsonFile,std::string outputFileName) // get splitted files and connect them
{
    this->downloadFilesFromCloud(fileName,jsonFile,outputFileName);
    connectSplittedFiles(outputFileName,19922944,outputFileName); // 19922944 - 19 MB
    fs::remove_all(outputFileName.substr(0,outputFileName.find_last_of('/')-1)); // remove temparary folder
}
void cloudBot::downloadFilesFromCloud(std::string filePath,std::string jsonFile,std::string outputPath) // get filePathes and download
{
    std::fstream f(jsonFile);
    boost::json::value json = boost::json::parse(f);
    std::vector<std::string> paths = getFilePathsFromJson(filePath,jsonFile);
    for(int i = 0;i<paths.size();i++)
    {
        downloadFile(paths[i],outputPath  + std::to_string(i));
    }
}