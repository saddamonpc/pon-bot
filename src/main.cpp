#include <string>
#include <time.h>

#include <dpp/dpp.h>
#include "ponMessages.h"

// Gets Discord Bot token from the text file
std::ifstream ifs("../bot_token.txt");
const std::string BOT_TOKEN( (std::istreambuf_iterator<char>(ifs) ),
                    (std::istreambuf_iterator<char>()    ) );

std::string returnMessages();

int main() {
    dpp::cluster bot(BOT_TOKEN);
    
    bot.on_log(dpp::utility::cout_logger());
    
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        // /ping command
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }

        // /pon and /procrastinate command
        if ((event.command.get_command_name() == "procrastinate") || 
        (event.command.get_command_name() == "pon")) {
            event.reply(returnMessages());
        }
    });
    
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // Register bot commands
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand("ping", "Ping pong!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("procrastinate", "Sends a message about procrastination.", bot.me.id));
            bot.global_command_create(dpp::slashcommand("pon", "Sends a message about procrastination.", bot.me.id));
        }
    });
    
    bot.start(dpp::st_wait);
}

// Function to return a random message from ponMessages.h
std::string returnMessages() {
    // Sets random seed according to current time
    srand(time(0));

    // Get a random message from messages in ponMessages.h
    return messages[rand() % (sizeof(messages)/sizeof(messages[0]))];
}