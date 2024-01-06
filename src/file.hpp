#ifndef FILE_H
#define FILE_H
typedef unsigned long long ull;
#include<fstream>
#include<iostream>
#include<filesystem>
void splitFilesIntoParts(std::string inputFileName,ull bufferSize,ull maxOutputFileSize,std::string outputFileName); // outputFile - все файлы будут иметь вид outputPath<n>
void connectSplittedFiles(std::string inputFile,ull bufferSize,std::string outputFile);
#endif