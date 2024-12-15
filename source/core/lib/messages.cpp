#include "messages.h"
#include "../common.h"

inline std::ostream& operator<<(std::ostream& os, const coms::SourceContext& sc) {
    os << sc.file << ":" << sc.line;
    return os;
}

namespace coms{

  void Warning( const std::string& msg,  const SourceContext& sc ){
    if(!LOGGING) return;
    std::cerr << YELLOW << "Warning: \"" + msg + "\""
              << RESET << "\n";
  }

  void Warning(const std::string &msg) {
    if(!LOGGING) return;
    std::cerr << YELLOW << "Warning: \"" + msg + "\""
              << "\n";
  }

  void Error(const std::string &msg, const SourceContext &sc) {
    if(!LOGGING) return;
    std::cerr << RED << "Severe error: \"" + msg + "\""
              << RESET << "\n";
    std::exit(EXIT_FAILURE);
  }

  void Error(const std::string &msg) {
    if(!LOGGING) return;
    std::cerr << RED << "Severe error: \"" + msg + "\""
              << RESET << "\n";    std::exit(EXIT_FAILURE);
  }

  void Message(const std::string &str) {
    if(!LOGGING) return;
    std::cout << GREEN << ">>> " + str << RESET << std::endl;
  }

} // namespace coms