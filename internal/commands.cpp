#include <filesystem>
#include <fstream>
#include <iostream>
#include <cstring>

#include "commands.hpp"
#include "../window/window.hpp"

std::string ExtractSubstring(const std::string& input) {
    size_t colonPos = input.find(':');
    
    if (colonPos != std::string::npos && colonPos < input.length() - 1) {
        return input.substr(colonPos + 1);
    } else {
        return "";
    }
}

bool HasSlash(const std::string &line)
{
    return line.find('/') != std::string::npos;
}

bool HasCommand(const std::string &line)
{
    return line.rfind("command:") != std::string::npos;
}

std::string GetExecutableName(const std::string &line)
{
    std::istringstream iss(line);
    std::string executable;
    iss >> executable;
    return executable;
}

bool VectorContains(const std::vector<std::string> &vec, const std::string &target)
{
    for (const std::string &item : vec)
    {
        if (item == target)
        {
            return true;
        }
    }
    return false;
}

std::vector<std::string> GetFuzzyCommands()
{
    std::vector<std::string> strings = {
        "fuzzy:about",
        "fuzzy:help",
        "command:"};

    std::string directoryPath = "/usr/share/applications";

    for (const auto &entry : std::filesystem::directory_iterator(directoryPath))
    {
        if (entry.path().extension() == ".desktop")
        {
            std::ifstream file(entry.path());
            std::string line;

            while (std::getline(file, line))
            {
                if (line.find("Exec=") != std::string::npos)
                {
                    std::string execStart = line.substr(line.find("Exec=") + 5);
                    if (HasSlash("/"))
                    {
                        size_t lastSlashPos = execStart.rfind('/');

                        execStart = execStart.substr(lastSlashPos + 1);
                    }
                    std::string executable = GetExecutableName(execStart);
                    if (!VectorContains(strings, executable))
                    {
                        strings.push_back(GetExecutableName(execStart));
                    }
                    break;
                }
            }
        }
    }

    return strings;
}

bool ExecuteCommandAsync(const std::string &command)
{
    const char *programName = command.c_str();

    pid_t pid = fork();

    if (pid == 0)
    {
        execlp(programName, programName, nullptr);
        return false;
    }
    else if (pid > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Process(const char *message, const char *image)
{
    std::string cppString(message);
    if (strcmp(message, "fuzzy:about") == 0)
    {
        ShowPopup("Fuzzy is made by netindev.\nA lightweight GUI alternative program to execute a command.", image, GTK_MESSAGE_INFO);
        return;
    }
    else if (strcmp(message, "fuzzy:help") == 0)
    {
        ShowPopup("Fuzzy automatically retrieve application information from the disk.\nJust type and it will be auto-completed.\n\nBut, if you preferer to run a command, not a app, type command:<the_command>.", image, GTK_MESSAGE_INFO);
        return;
    }
    if (HasCommand(message))
    {
        message = ExtractSubstring(message).c_str();
    }
    else if (!VectorContains(GetFuzzyCommands(), message))
    {
        ShowPopup("It doesn't seems like a runnable application.", image, GTK_MESSAGE_ERROR);
        return;
    }
    bool value = ExecuteCommandAsync(message);
    if (value)
    {
        exit(1);
    }
    else
    {
        ShowPopup("Invalid command, try again.", image, GTK_MESSAGE_ERROR);
    }
}