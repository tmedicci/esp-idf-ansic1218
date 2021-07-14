#ifndef LIB_EXCEPTION_H
#define LIB_EXCEPTION_H

#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ansic1218 {

	class Exception : public std::runtime_error {

		template<typename Arg>
		void push(std::ostringstream &out, Arg &&arg) {
			out << arg;
		}

		template<typename Arg, typename ... Args>
		void push(std::ostringstream &out, Arg &&arg, Args &&... args) {
			out << arg;
			push(out, args...);
		}

		template<typename Arg, typename... Args>
		std::string build(Arg &&arg, Args &&... args) {
			std::ostringstream out;
			out << arg;
			prettify(out);
			push(out, args...);
			return out.str();
		}

		void prettify(std::ostringstream &out) {
			const std::string scope("::");
			auto s = out.str();
			auto scope_pos = s.rfind(scope);
			if (scope_pos == std::string::npos) return;
			scope_pos += scope.size();
			auto args_pos = s.find('(', scope_pos);
			if (args_pos == std::string::npos) return;
			auto name = s.substr(scope_pos, args_pos - scope_pos);
			out.str("");
			out.clear();
			out << name << ": ";
		}

	public:
		template<typename Arg, typename... Args>
		explicit Exception(Arg &&pretty_function, Args &&... args) : runtime_error(build(pretty_function, args...)) {}
	};
}

#endif //LIB_EXCEPTION_H
