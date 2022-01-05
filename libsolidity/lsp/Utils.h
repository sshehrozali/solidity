/*
	This file is part of solidity.

	solidity is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	solidity is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with solidity.  If not, see <http://www.gnu.org/licenses/>.
*/
// SPDX-License-Identifier: GPL-3.0

#pragma once

#include <liblangutil/SourceLocation.h>

#include <libsolidity/ast/ASTForward.h>

#include <libsolutil/JSON.h>

#include <optional>
#include <vector>

#if !defined(NDEBUG)
#include <fstream>
#define lspDebug(message) (std::ofstream("/tmp/solc.log", std::ios::app) << (message) << std::endl)
#else
#define lspDebug(message) do {} while (0)
#endif

namespace solidity::langutil
{
class CharStreamProvider;
}

namespace solidity::lsp
{

class FileRepository;

std::optional<langutil::LineColumn> parseLineColumn(Json::Value const& _lineColumn);
Json::Value toJson(langutil::LineColumn _pos);
Json::Value toJsonRange(langutil::LineColumn const& _start, langutil::LineColumn const& _end);

std::vector<frontend::Declaration const*> allAnnotatedDeclarations(frontend::Expression const* _expression);
std::optional<langutil::SourceLocation> declarationLocation(frontend::Declaration const* _declaration);

}
