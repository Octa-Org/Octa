/////////////////////////////////////////////////////////////
///                                                       ///
///     ██████╗ ██╗███████╗████████╗                      ///
///     ██╔══██╗██║██╔════╝╚══██╔══╝                      ///
///     ██████╔╝██║█████╗     ██║                         ///
///     ██╔══██╗██║██╔══╝     ██║                         ///
///     ██║  ██║██║██║        ██║                         ///
///     ╚═╝  ╚═╝╚═╝╚═╝        ╚═╝                         ///
///     * RIFT CORE - The official compiler for Rift.     ///
///     * Copyright (c) 2024, Rift-Org                    ///
///     * License terms may be found in the LICENSE file. ///
///                                                       ///
/////////////////////////////////////////////////////////////


#pragma once

#include <memory>
#include <vector>
#include <exception>
#include <scanner/tokens.hh>
#include <reader/reader.hh>
#include <ast/grmr.hh>
#include <ast/expr.hh>
#include <ast/stmt.hh>

using namespace rift::scanner;
using namespace rift::reader;

namespace rift
{
    namespace ast
    {
        class Stmt;

        /// @class Parser
        /// @brief The parser class is responsible for parsing the tokens generated by the scanner.
        class Parser : public Reader<Token>
        {
            public:
                Parser(std::shared_ptr<std::vector<Token>> &tokens) : Reader<Token>(tokens), tokens(tokens)  {};
                ~Parser() = default;

                /// @brief Parses the tokens and returns an expression
                std::unique_ptr<Program> parse();
            protected:
                std::shared_ptr<std::vector<Token>> tokens;
                std::exception exception;

            private:
                #pragma mark - Grammar Evaluators
                
                /// @note rules in order of precedence <Expr>

                /// @example 1 + 2 * 3
                std::unique_ptr<Expr> expression();
                /// @example 1 == 1, 1 != 2
                std::unique_ptr<Expr> equality();
                /// @example 1 > 2, 1 <= 2
                std::unique_ptr<Expr> comparison();
                /// @example 1 + 2, 1 - 2
                std::unique_ptr<Expr> term();
                /// @example 1 * 2, 1 / 2
                std::unique_ptr<Expr> factor();
                /// @example -1, !1
                std::unique_ptr<Expr> unary();
                /// @example 1, "string", true, false, nil
                std::unique_ptr<Expr> primary();

                /// @note rules in order of precedence <Stmt>

                /// @example function();
                std::unique_ptr<StmtExpr> statement_expression();
                /// @example print(1 + 2);
                std::unique_ptr<StmtPrint> statement_print();

                /// @note program

                std::unique_ptr<Program> program();
                
                /// @brief Syncronizes the parser to avoid error-cascading
                void synchronize();
        };

        class ParserException : public ReaderException
        {
            public:
                ParserException(const std::string &message) : message(message) {}
                ~ParserException() = default;

                const char *what() const noexcept override { return message.c_str(); }

            private:
                std::string message;
        };
    }
}