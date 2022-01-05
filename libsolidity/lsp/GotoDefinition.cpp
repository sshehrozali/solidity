#include <libsolidity/lsp/GotoDefinition.h>
#include <libsolidity/lsp/Transport.h> // for RequestError
#include <libsolidity/lsp/Utils.h>
#include <libsolidity/ast/AST.h>
#include <libsolidity/ast/ASTUtils.h>

#include <fmt/format.h>

#include <memory>
#include <string>
#include <vector>

using namespace solidity::frontend;
using namespace solidity::langutil;
using namespace solidity::lsp;
using namespace std;

void GotoDefinition::operator()(MessageID _id, Json::Value const& _args)
{
	auto const [sourceUnitName, lineColumn] = extractSourceUnitNameAndLineColumn(_args);

	ASTNode const* sourceNode = m_server.requestASTNode(sourceUnitName, lineColumn);
	lspDebug(fmt::format("GotoDefinition: {}", typeid(*sourceNode).name()));

	vector<SourceLocation> locations;
	if (auto const* expression = dynamic_cast<Expression const*>(sourceNode))
	{
		// Handles all expressions that can have one or more declaration annotation.
		for (auto const* declaration: allAnnotatedDeclarations(expression))
			if (auto location = declarationLocation(declaration); location.has_value())
				locations.emplace_back(move(location.value()));
	}
	else if (auto const* identifierPath = dynamic_cast<IdentifierPath const*>(sourceNode))
	{
		if (auto const* declaration = identifierPath->annotation().referencedDeclaration)
			if (auto location = declarationLocation(declaration); location.has_value())
				locations.emplace_back(move(location.value()));
	}
	else if (auto const* importDirective = dynamic_cast<ImportDirective const*>(sourceNode))
	{
		auto const& path = *importDirective->annotation().absolutePath;
		if (fileRepository().sourceUnits().count(path))
			locations.emplace_back(SourceLocation{0, 0, make_shared<string const>(path)});
	}
	else if (auto const* declaration = dynamic_cast<Declaration const*>(sourceNode))
	{
		if (auto location = declarationLocation(declaration); location.has_value())
			locations.emplace_back(move(location.value()));
	}

	Json::Value reply = Json::arrayValue;
	for (SourceLocation const& location: locations)
		reply.append(toJson(location));
	client().reply(_id, reply);
}
