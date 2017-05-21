#ifndef __BOOLEANEXPRESSION_H__
#define __BOOLEANEXPRESSION_H__

#pragma once

#include <iostream>
#include <string>
#include <vector>

class BooleanExpression;

/*! \brief Operator care permite citirea unei expresii boole-ene dintr-un
 * istream. Se citeste o singura linie in intregime si se parseaza.
 */
std::istream& operator>> (std::istream& in, BooleanExpression& right);

class BooleanExpression{
 public:
  /*! \brief Valori intregi constante pentru tipurile de lexeme ce pot aparea
   * intr-o expresie: <b>True</b>, <b>False</b>, <b>And</b>, <b>Or</b> si
   * <b>Xor</b>.
   */
  enum Lexem{
    True,
    False,
    And,
    Or,
    Xor
  };

 private:
  std::vector<Lexem> expr;

  bool lexical_analysis(const std::string& input);
  bool semantic_analysis();
  bool parse(const std::string& input);  
  bool valid;

 public:
  /*! \brief Functie care verifica daca ultima expresie parsata a fost valida
   * sau nu din punct de vedere gramatical.
   * \returns Raspunsul ultimei operatii de parsare.
   */
  inline bool is_valid(){
    return valid;
  }

  /*! \brief Operator care reparseaza o expresie dintr-un string.
   */
  inline void operator<< (const char* input){
    parse(std::string(input));
  }

  /*! \brief Operator care reparseaza o expresie dintr-un string.
   */
  inline void operator<< (const std::string& input){
    parse(input);
  }

  /*! \brief Functie pentru acces la vectorul de lexeme rezultat in urma
   * parsarii unei expresii.
   * \returns Vectorul de lexeme, daca ultimul string parsata a fost corect
   * gramatical, sau un vector gol altfel.
   */
  inline std::vector<Lexem> to_vector(){
    return valid ? expr : std::vector<Lexem>();
  }

  friend std::istream& operator>> (std::istream& in, BooleanExpression& right);

  /*! \brief Constructor care initializeaza o expresie vida (si deci, invalida).
   * Ca sa o puteti folosi, dati-i un string de parsat.
   */
  BooleanExpression() : valid(false) { }
};

#endif

