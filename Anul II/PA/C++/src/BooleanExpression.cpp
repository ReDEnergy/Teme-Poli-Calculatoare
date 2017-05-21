#include <cstdlib>
#include <cstring>
#include <iostream>

#include "BooleanExpression.h"

std::istream& operator>> (std::istream& in, BooleanExpression& right)
{
  std::string line;
  if (in == std::cin){
    std::cout << "Dati expresia de parsat (pe o singura linie!):\n";
  }
  getline(in, line);
  right.parse(line);
  return in;
}

bool BooleanExpression::lexical_analysis(const std::string& input)
{
  unsigned int parsed = 0, trailing;
  std::string lexem;

  std::cerr << "Lexical analysis... ";
  while (parsed < input.size()){

    /* Avansam cu parsed peste spatiile albe */
    parsed = input.find_first_not_of(" \t\n", parsed);
    if (parsed == std::string::npos){
      break;
    }

    /* Avansam cu trailing peste spatiile non-albe */
    trailing = input.find_first_of(" \t\n", parsed);
    if (trailing == std::string::npos){
      trailing = input.size();
    }

    /* Verificam lexema detectata */
    lexem = input.substr(parsed, trailing - parsed);

    if (lexem == "true"){
      expr.push_back(True);
    } else if (lexem == "false"){
      expr.push_back(False);
    } else if (lexem == "and"){
      expr.push_back(And);
    } else if (lexem == "or"){
      expr.push_back(Or);
    } else if (lexem == "xor"){
      expr.push_back(Xor);
    } else {
      std::cerr << "ERROR!\n\t\"" << lexem
          << "\" is not a valid keyword.\n";
      return false;
    }

    parsed = trailing;
  }

  if (expr.size() == 0){
    std::cerr << "ERROR!\n\tExpression cannot be void.\n";
    return false;
  } else {
    std::cerr << "OK!\n";
    return true;
  }
}

bool BooleanExpression::semantic_analysis()
{
  std::cerr << "Semantic analysis... ";
  for (unsigned int i = 0; i < expr.size(); ++i){
    if ((i % 2 == 0) && expr[i] != True && expr[i] != False){
      std::cerr << "ERROR!\n\tAn operator must be placed between "
          << "two operands.\n";
      return false;
    } 
    if ((i % 2 == 1) && expr[i] != And && expr[i] != Or && expr[i] != Xor){
      std::cerr << "ERROR!\n\tAn operand cannot follow another operand.\n";
      return false;
    }
  }

  if (expr.size() % 2 == 0){
    std::cerr << "ERROR!\n\tThe expression cannot end with an operator.\n";
    return false;
  } 

  std::cerr << "OK!\n";
  return true;
}

bool BooleanExpression::parse(const std::string& input)
{
  expr.clear();
  if (lexical_analysis(input) == false || semantic_analysis() == false){
    expr.clear();
    valid = false;
  } else {
    valid = true;
  }
  return valid;
}

