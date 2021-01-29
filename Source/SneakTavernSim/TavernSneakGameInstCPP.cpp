// Fill out your copyright notice in the Description page of Project Settings.


#include "TavernSneakGameInstCPP.h"

/** Obtain all files in a provided directory, with optional extension filter. All files are returned if Ext is left blank. Returns false if operation could not occur. */
bool UTavernSneakGameInstCPP::GetSaveGameFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
    if (RootFolderFullPath.Len() < 1) return false; //if RootFolderFullPath is null

    FPaths::NormalizeDirectoryName(RootFolderFullPath); //converts all / and \ to text /

    IFileManager& FileManager = IFileManager::Get(); //gets the filemanager class

    if (Ext == "") //check if Ext is null || Ext is used for a specific file type e.g. txt
    {
        Ext = "*.*";
    }
    else
    {
        Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
    }

    FString FinalPath = RootFolderFullPath + "/" + Ext;
    FileManager.FindFiles(Files, *FinalPath, true, false);
    return true;
}