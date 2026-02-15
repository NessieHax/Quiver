#pragma once

#include <filesystem>
#include <memory>
#include "PCK/PCKAssetFile.h"
#include "UI/Menu/MenuFunctions.h"

struct FileTreeNode
{
	FileTreeNode(std::string path, std::shared_ptr<PCKAssetFile> file)
		: path(path), file(file)
	{}

	std::string path;
	std::shared_ptr<PCKAssetFile> file;
	std::vector<FileTreeNode> children;
};

void SavePCK(std::shared_ptr<PCKFile> &pckFile, const FileTreeNode &rootnode, const std::filesystem::path &path, const std::string &defaultName);

void SaveFolderAsFiles(const FileTreeNode &node, bool includeProperties = false);

// Convert file tree to PCK File collection
void TreeToPCKFileCollection(std::shared_ptr<PCKFile> &pckFile, const FileTreeNode &rootNode);

// Finds a node by path in a given file tree
FileTreeNode *FindNodeByPath(const std::string &path, std::vector<FileTreeNode> &nodes);

// Deletes a node in a given file tree
void DeleteNode(const FileTreeNode &targetNode, std::vector<FileTreeNode> &nodes);

// Sorts a given file tree
void SortTree(FileTreeNode &node);

// Builds a given file tree
void BuildFileTree(const std::vector<std::shared_ptr<PCKAssetFile>> &assets, FileTreeNode &root);

// Scrolls to selected node when not visible
void ScrollToNode(bool &keyScrolled);