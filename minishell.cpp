#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#include <cstdlib>
#include <algorithm>
#include <readline/readline.h>
#include <readline/history.h>
#include <ctime>
#include <iomanip>

// ANSI Color Codes for styling
#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define DIM         "\033[2m"
#define UNDERLINE   "\033[4m"
#define BLINK       "\033[5m"
#define REVERSE     "\033[7m"

// Text Colors
#define BLACK       "\033[30m"
#define RED         "\033[31m"
#define GREEN       "\033[32m"
#define YELLOW      "\033[33m"
#define BLUE        "\033[34m"
#define MAGENTA     "\033[35m"
#define CYAN        "\033[36m"
#define WHITE       "\033[37m"

// Bright Colors
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

class MiniShell {
private:
    std::vector<std::string> commandHistory;
    std::string historyFile;
    bool running;
    std::string username;
    std::string hostname;

public:
    MiniShell() : historyFile(".minishell_history"), running(true) {
        using_history();
        loadHistory();
        initializeUserInfo();
        setupTerminal();
    }

    ~MiniShell() {
        saveHistory();
        resetTerminal();
    }

    void initializeUserInfo() {
        const char* user = getenv("USER");
        if (!user) user = getenv("USERNAME");
        if (!user) user = "user";
        username = std::string(user);

        char hostbuffer[256];
        if (gethostname(hostbuffer, sizeof(hostbuffer)) == 0) {
            hostname = std::string(hostbuffer);
        } else {
            hostname = "localhost";
        }
    }

    void setupTerminal() {
        std::cout << "\033[2J\033[H";
        std::cout << "\033]0;MiniShell - Modern Terminal\007";
        printWelcomeBanner();
    }

    void resetTerminal() {
        std::cout << RESET;
    }

    void printWelcomeBanner() {
        std::cout << BRIGHT_CYAN << BOLD;
        std::cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
        std::cout << "║                          🚀 Welcome to MiniShell!                            ║\n";
        std::cout << "╚══════════════════════════════════════════════════════════════════════════════╝" << RESET << std::endl;
        std::cout << std::endl;
    }

    void loadHistory() {
        std::ifstream file(historyFile);
        std::string line;
        if (file.is_open()) {
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    commandHistory.push_back(line);
                    add_history(line.c_str());
                }
            }
            file.close();
        }
    }

    void saveHistory() {
        std::ofstream file(historyFile);
        if (file.is_open()) {
            for (const auto& cmd : commandHistory) {
                file << cmd << std::endl;
            }
            file.close();
        }
    }

    void addToHistory(const std::string& command) {
        if (!command.empty() && command != "history" && command != "clearhistory") {
            commandHistory.push_back(command);
            add_history(command.c_str());
        }
    }

    std::vector<std::string> parseCommand(const std::string& input) {
        std::vector<std::string> tokens;
        std::istringstream iss(input);
        std::string token;
        while (iss >> token) {
            tokens.push_back(token);
        }
        return tokens;
    }

    bool isBuiltinCommand(const std::string& command) {
        return command == "exit" || command == "cd" || command == "pwd" || 
               command == "history" || command == "clearhistory" || command == "help" || 
               command == "clear";
    }

    void executeBuiltinCommand(const std::vector<std::string>& tokens) {
        const std::string& command = tokens[0];

        if (command == "exit") {
            running = false;
            std::cout << BRIGHT_MAGENTA << "👋 Goodbye!" << RESET << std::endl;
        }
        else if (command == "cd") {
            if (tokens.size() < 2) {
                const char* home = getenv("HOME");
                if (home && chdir(home) != 0) {
                    std::cout << BRIGHT_RED << "❌ Error: could not change to home directory" << RESET << std::endl;
                }
            } else {
                if (chdir(tokens[1].c_str()) != 0) {
                    std::cout << BRIGHT_RED << "❌ Error: could not change to directory '" << tokens[1] << "'" << RESET << std::endl;
                }
            }
        }
        else if (command == "pwd") {
            char* cwd = getcwd(nullptr, 0);
            if (cwd) {
                std::cout << BRIGHT_BLUE << "📁 " << BRIGHT_WHITE << cwd << RESET << std::endl;
                free(cwd);
            }
        }
        else if (command == "history") {
            for (size_t i = 0; i < commandHistory.size(); i++) {
                std::cout << i + 1 << ": " << commandHistory[i] << std::endl;
            }
        }
        else if (command == "clearhistory") {
            commandHistory.clear();
            clear_history();
            std::ofstream file(historyFile, std::ofstream::trunc);
            file.close();
            std::cout << BRIGHT_GREEN << "✅ History cleared!" << RESET << std::endl;
        }
        else if (command == "help") {
            std::cout << BRIGHT_CYAN << "Built-in commands: exit, cd, pwd, history, clearhistory, clear, help" << RESET << std::endl;
        }
        else if (command == "clear") {
            std::cout << "\033[2J\033[H";
        }
    }

    void executeExternalCommand(const std::vector<std::string>& tokens) {
        std::vector<char*> args;
        for (const auto& token : tokens) {
            args.push_back(const_cast<char*>(token.c_str()));
        }
        args.push_back(nullptr);

        pid_t pid = fork();
        if (pid == 0) {
            if (execvp(args[0], args.data()) == -1) {
                std::cout << BRIGHT_RED << "❌ Error: command '" << tokens[0] << "' not found" << RESET << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        else if (pid > 0) {
            int status;
            waitpid(pid, &status, 0);
        }
        else {
            std::cout << BRIGHT_RED << "❌ Error: could not create process" << RESET << std::endl;
        }
    }

    // 🔥 FIXED: return prompt string instead of setting rl_set_prompt
    std::string printPrompt() {
        char* cwd = getcwd(nullptr, 0);
        std::string currentDir = "/";
        if (cwd) {
            currentDir = std::string(cwd);
            free(cwd);
        }

        std::string prompt = std::string(BRIGHT_CYAN) + "┌─[" + 
                           std::string(BRIGHT_GREEN) + username + 
                           std::string(BRIGHT_WHITE) + "@" + 
                           std::string(BRIGHT_BLUE) + hostname + 
                           std::string(BRIGHT_CYAN) + "]─[" + 
                           std::string(BRIGHT_YELLOW) + currentDir + 
                           std::string(BRIGHT_CYAN) + "]\n└─" + 
                           std::string(BRIGHT_MAGENTA) + "🚀 " + 
                           std::string(BRIGHT_WHITE) + "$ " + 
                           std::string(RESET);

        return prompt;
    }

    void run() {
        char* input;
        while (running) {
            std::string prompt = printPrompt();
            input = readline(prompt.c_str());

            if (!input) {
                std::cout << std::endl << BRIGHT_MAGENTA << "👋 Goodbye!" << RESET << std::endl;
                break;
            }

            std::string command(input);
            free(input);

            if (command.empty()) continue;

            addToHistory(command);

            std::vector<std::string> tokens = parseCommand(command);

            if (tokens.empty()) continue;

            if (isBuiltinCommand(tokens[0])) {
                executeBuiltinCommand(tokens);
            } else {
                executeExternalCommand(tokens);
            }
        }
        saveHistory();
    }
};

int main() {
    MiniShell shell;
    shell.run();
    return 0;
}
