#include"file.hpp"
namespace fs = std::filesystem;
void splitFilesIntoParts(std::string inputFileName,ull bufferSize,ull maxOutputFileSize,std::string outputFileName) // OutputFile - все файлы будут иметь вид outputPath<n>
{
    char* buffer = new char[bufferSize]; // Creating of buffer
    std::ifstream fileStream(inputFileName);
    ull j = 0; // counter for splitted files
    fileStream.seekg(0,std::ios::end);
    ull fileSize = fileStream.tellg();
    fileStream.seekg(0); // getting fileSize
    while(fileStream.peek() != EOF)
    {
        std::ofstream oFile(outputFileName + std::to_string(j)); // making new split file
        for(ull i = 0;i<maxOutputFileSize;)
        {
            ull writeSize = 0;
            if(fileSize - fileStream.tellg() < bufferSize) // if left file data < buffer 
            {
                writeSize = fileSize - fileStream.tellg();
                fileStream.read(buffer,writeSize);
                oFile.write(buffer,writeSize);
                fileStream.seekg(0,std::ios_base::end); // ending loop
                break;
            }
            else writeSize = bufferSize;
            fileStream.read(buffer,writeSize);
            oFile.write(buffer,writeSize);
            i+=writeSize;
        }
        oFile.close();
        j++;
    }
    delete[] buffer; //deleting buffer
}
void connectSplittedFiles(std::string inputFile,ull bufferSize,std::string outputFile)
{
    std::ofstream file(outputFile);
    char* buffer = new char[bufferSize];
    for(int i = 0;fs::exists(inputFile + std::to_string(i));i++) //reading all file inputFile<i> 
    {
        std::ifstream p(inputFile + std::to_string(i));
        char* buffer = new char[bufferSize];
        p.seekg(0,std::ios_base::end);
        ull fileSize = p.tellg();
        p.seekg(0); //getting fileSize
        while(p.peek() != EOF)
        {
            ull writeSize = 0;
            if(fileSize - p.tellg() < bufferSize) writeSize = fileSize%bufferSize; // ending loop
            else writeSize = bufferSize;
            p.read(buffer,bufferSize);
            file.write(buffer,writeSize);
        }
    }
    file.close();
    delete[] buffer;
}