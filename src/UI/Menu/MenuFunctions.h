#pragma once
#include <sstream>
#include <string>
#include <filesystem>
#include "IO/IO.h"
#include "PCK/PCKFile.h"
#include "PCK/PCKAssetFile.h"

// function for opening a PCK file via dialog
std::filesystem::path OpenPCKFileDialog();

// function for saving a PCK File to disk
void SavePCKFile(std::shared_ptr<PCKFile> &pckFile, const std::filesystem::path &outpath);

// function for saving a PCK File to a specified point on disk
void SavePCKFileDialog(std::shared_ptr<PCKFile> &pckFile, const std::string &defaultName);

// function to open a file dialog and read properties into a given file
void SetFilePropertiesDialog(PCKAssetFile &file);

// function to set data from file; returns success bool for processing
bool SetDataFromFile(PCKAssetFile &file);

// Save File Properties to File via path
void SaveFilePropertiesToFile(const PCKAssetFile &file, const std::filesystem::path &outpath);

// Save File Properties to file via dialog
void SaveFilePropertiesDialog(const PCKAssetFile &file);

// Save File Data to file via Dialog
void ExtractFileDataDialog(const PCKAssetFile &file, bool includeProperties = false);