#include <functional>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>

struct my_settings {
    bool help{false};
    bool verbose{false};
    std::optional<std::string> infile;
    std::optional<std::string> outfile;
    std::optional<int> value;
};

typedef std::function<void(my_settings &)> flag_handler;
typedef std::function<void(my_settings &, const std::string &)> arg_handler;

const std::unordered_map<std::string, flag_handler> flags{

    {"--help", [](my_settings &s) { s.help = true; }},
    {"-h", [](my_settings &s) { s.help = true; }},

    {"--verbose", [](my_settings &s) { s.verbose = true; }},
    {"-v", [](my_settings &s) { s.verbose = true; }},

    {"--quiet", [](my_settings &s) { s.verbose = false; }},
};

const std::unordered_map<std::string, arg_handler> arguments{

    {"--output", [](my_settings &s, const std::string &arg) { s.outfile = arg; }},
    {"-o", [](my_settings &s, const std::string &arg) { s.outfile = arg; }},

    {"--value", [](my_settings &s, const std::string &val) { s.value = std::stoi(val); }},
};

void print_settings(my_settings &s) {
    std::cout << "help: " << s.help << "\n";
    std::cout << "verbose: " << s.verbose << "\n";
    std::cout << "infile: " << s.infile.value_or("empty") << "\n";
    std::cout << "outfile: " << s.outfile.value_or("empty") << "\n";
    std::cout << "value: " << s.value.value_or(NULL) << "\n";
};

my_settings parse_settings(int argc, char const *argv[]) {
    my_settings settings;

    for (int i{1}; i < argc; i++) {
        // gets the option to process
        std::string opt{argv[i]};

        if (auto j{flags.find(opt)}; j != flags.end())
            j->second(settings);

        else if (auto k{arguments.find(opt)}; k != arguments.end()) {
            if (i++ < argc)
                k->second(settings, {argv[i]});
            else
                throw std::runtime_error{"missing param after" + opt};

        } else if (!settings.infile)
            settings.infile = argv[i];
        else
            std::cerr << "unrecognized command line option " << opt << std::endl;
    };

    return settings;
}

int main(int argc, char const *argv[]) {
    my_settings settings = parse_settings(argc, argv);
    print_settings(settings);
}
