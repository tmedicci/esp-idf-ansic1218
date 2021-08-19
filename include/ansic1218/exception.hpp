// Copyright 2021 Time Energy Industria e Comercio Ltda
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
