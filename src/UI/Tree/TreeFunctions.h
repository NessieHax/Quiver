#pragma once

<<<<<<< HEAD
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
=======
#include "Application/Application.h"
#include "Binary/Binary.h"
#include "PCK/PCKAssetFile.h"
#include "UI/Tree/TreeNode.h"

// Saves PCK file from tree nodes
void SavePCK(std::vector<FileTreeNode> nodes, Binary::Endianness endianness, const std::string& path = "", const std::string& defaultName = "");

// Writes folder of nodes to disk via file dialog
void WriteFolder(const FileTreeNode& node, bool includeProperties = false);
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

// Convert file tree to PCK File collection
void TreeToPCKFileCollection(std::shared_ptr<PCKFile> &pckFile, const FileTreeNode &rootNode);

// Finds a node by path in a given file tree
FileTreeNode *FindNodeByPath(const std::string &path, std::vector<FileTreeNode> &nodes);

// Renames a directory
void RenameDirectory(const std::string& targetPath, const std::string& newName, std::vector<FileTreeNode>& nodes);

// Deletes a node in a given file tree
void DeleteNode(const FileTreeNode &targetNode, std::vector<FileTreeNode> &nodes);

// Sorts a given file tree
void SortTree(FileTreeNode &node);

// Builds a given file tree
<<<<<<< HEAD
void BuildFileTree(const std::vector<std::shared_ptr<PCKAssetFile>> &assets, FileTreeNode &root);
=======
void BuildFileTree();
>>>>>>> 5aede736572f73458824482c4fcb81b6ac43a54d

// Scrolls to selected node when not visible
void ScrollToNode(bool &keyScrolled);