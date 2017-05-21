#ifndef __MATRIX_H__
#define __MATRIX_H__

#pragma once

#include <iostream>
#include <cstdlib>
#include <cstring>

template<class Type> class Matrix;

/*! \brief Operatorul de inmultire efectueaza produsul a doua matrice. Atentie,
 * inmultirea nu este comutativa!
 * \param left Matricea din stanga operatorului de inmultire 
 * \param right Matricea din dreapta operatorului de inmultire
 * \return Daca inmultirea a fost legala, intoarce produsul matricelor. Altfel,
 * daca inmultirea nu a fost legala (pentru a se putea inmulti, matricele
 * trebuie sa aiba numar de coloane, respectiv linii corespunzatoare) se
 * intoarce o matrice cu 0 linii si 0 coloane.
 */
template<class Type>
Matrix<Type> operator* (const Matrix<Type>& left,const Matrix<Type>& right)
{
  if (left.n_col != right.n_lin){
    std::cerr << "Warning! Ingnoring \"*\" between matrices of sizes ("
        << left.n_lin << "," << left.n_col << ") and (" << right.n_lin
        << "," << right.n_col << ").\n";
    return Matrix<Type>();
  } else {
    Matrix<Type> result(left.n_lin, right.n_col, Matrix<Type>::Zero);
    for (int i = 0; i < left.n_lin; i++){
      for (int j = 0; j < right.n_col; j++){
        for (int k = 0; k < left.n_col; k++){
          result[i][j] += left[i][k] * right[k][j];
        }
      }
    }
    return result;
  }
}

/*! \brief Operatorul de adunare efectueaza suma a doua matrice. Suma
 * matricelor este comutativa.
 * \param left Matricea din stanga operatorului de adunare
 * \param right Matricea din dreapta operatorului de adunare
 * \return Daca adunarea a fost legala, intoarce suma matricelor. Altfel,
 * daca adunarea nu a fost legala (pentru a se putea aduna, matricele trebuie
 * sa aiba dimensiuni identice) se intoarce o matrice cu 0 linii si 0 coloane.
 */

template<class Type>
Matrix<Type> operator+ (const Matrix<Type>& left, const Matrix<Type>& right) 
{
  if (left.n_lin != right.n_lin || left.n_col != right.n_col){
    std::cerr << "Warning! Ingnoring \"+\" between matrices of sizes ("
        << left.n_lin << "," << left.n_col << ") and (" << right.n_lin
        << "," << right.n_col << ").\n";
    return Matrix<Type>();
  } else {
    Matrix<Type> result(left.n_lin, right.n_col, Matrix<Type>::Zero);
    for (int i = 0; i < left.n_lin; i++){
      for (int j = 0; j < right.n_col; j++){
        result[i][j] = left[i][j] + right[i][j];
      }
    }
    return result;
  }
}

/*! \brief Operatorul de scadere efectueaza diferenta a doua matrice.
 * Diferenta matricelor nu este comutativa.
 * \param left Matricea din stanga operatorului de scadere
 * \param right Matricea din dreapta operatorului de scadere
 * \return Daca scaderea a fost legala, intoarce diferenta matricelor. Altfel,
 * daca scaderea nu a fost legala (pentru a se putea scadea, matricele trebuie
 * sa aiba dimensiuni identice) se intoarce o matrice cu 0 linii si 0 coloane.
 */
template<class Type>
Matrix<Type> operator- (const Matrix<Type>& left, const Matrix<Type>& right)
{
  if (left.n_lin != right.n_lin || left.n_col != right.n_col){
    std::cerr << "Warning! Ingnoring \"+\" between matrices of sizes ("
        << left.n_lin << "," << left.n_col << ") and (" << right.n_lin
        << "," << right.n_col << ").\n";
    return Matrix<Type>();
  } else {
    Matrix<Type> result(left.n_lin, right.n_col, Matrix<Type>::Zero);
    for (int i = 0; i < left.n_lin; i++){
      for (int j = 0; j < right.n_col; j++){
        result[i][j] = left[i][j] - right[i][j];
      }
    }
    return result;
  }
}

/*! \brief Operatorul de deserializare realizeaza citirea unei matrice
 * dintr-un fisier (sau de la tastatura). <b> Atentie!</b> Se citesc doar
 * datele, dimensiunile sunt cele date in constructor.
 * \param in Stream-ul (fisier sau intrare standard) de unde se citeste
 * \param right Matricea al carei continut este citit
 * \return Deserializarea intoarce o referinta la flux, pentru a putea
 * permite chaining-ul.
 */
template<class Type>
std::istream& operator>> (std::istream& in, Matrix<Type> &right)
{
  for (int i = 0; i < right.n_lin; i++){
    for (int j = 0; j < right.n_col; j++){
      in >> right[i][j];
    }
  }
  return in;
}

/*! \brief Operatorul de serializare realizeaza scrierea unei matrice intr-un
 * fisier (sau pe ecran). <b> Atentie!</b> Se scrie doar continutul, nu si
 * dimensiunile matricei.
 * \param out Stream-ul (fisier sau iesire standard) unde se scrie
 * \param right Matricea al carei continut este scris
 * \return Serializarea intoarce o referinta la flux, pentru a putea permite
 * chaining-ul.
 */
template<class Type>
std::ostream& operator<< (std::ostream& out, Matrix<Type> right)
{
  for (int i = 0; i < right.n_lin; i++){
    for (int j = 0; j < right.n_col; j++){
      out << (j == 0 ? "" : " ") << right[i][j];
    }
    out << std::endl;
  }
  return out;
}

template <class Type>
class Matrix{
 private:
  int n_lin, n_col;
  Type* m;

  void deallocate() {
    if (n_lin | n_col){
      delete[] m;
      n_lin = n_col = 0;
    }
  }

  void allocate(int init) {
    m = new Type[n_lin*n_col];
    if (init == Zero){
      // Set all the memory to 0
      memset(m, 0x00, sizeof(Type)*n_col*n_lin);
    } else if (init == Unit && n_lin == n_col){
      // Set the matrix as a unit matrix
      memset(m, 0x00, sizeof(Type)*n_col*n_lin);
      for (int i = 0; i < n_lin; i++){
        m[i*n_col+i] = 1;
      }
    } 
  }

  void allocate(const Matrix<Type>& right) {
    m = new Type[n_lin*n_col];
    memcpy(m, right.m, sizeof(Type)*n_col*n_lin);
  }

 public:
  /*! \brief Constructor pentru instantierea unei matrice de <i>n_lin</i>
   * linii, respectiv <i>n_col</i> coloane. Al treilea parametru este optional,
   * dar cu ajutorul lui se poate initializa matricea nou construita.
   * \param n_lin Numarul de linii al matricei
   * \param n_col Numarul de coloane al matricei
   * \param init O constanta dintre <b>Matrix::None</b>, <b>Matrix::Unit</b> sau
   * <b>Matrix::Zero</b>. Specificarea oricarei alte valori inafara de acestea
   * trei face fallback pe <b>Matrix::None</b>.
   */
  Matrix(int n_lin, int n_col, int init = None):
      n_lin(n_lin),
      n_col(n_col) {
        if (n_lin > 0 && n_col > 0){
          allocate(init);
        } else {
          n_lin = 0;
          n_col = 0;
          m = NULL;
        }
      }

  /*! \brief Copy constructor pentru clonarea unei matrice (atat dimensiuni,
   * cat si continut)
   * \param right Matricea care se cloneaza
   */
  Matrix(const Matrix<Type>& right):
      n_lin(right.n_lin),
      n_col(right.n_col) {
        allocate(right);
      }

  /*! \brief Constructor care creaza o matrice de 0 lini si 0 coloane (care
   * nu poate avea continut). In mod normal, acest rezultat intors de o
   * operatie semnaleaza o eroare de aritmetica matricelor.
   */
  Matrix():
      n_lin(0),
      n_col(0),
      m(NULL) {
      }

  /*! \brief Getter pentru a afla numarul de linii al matricei
   */
  inline int get_n_lin() const { 
    return n_lin;
  }

  /*! \brief Getter pentru a afla numarul de coloane al matricei
   */
  inline int get_n_col() const {
    return n_col;
  }

  ~Matrix() {
    deallocate();
  }

  /*! \brief Constanta statica care identifica tipul de matrice nula (utila
   * pentru a o trimite in constructor)
   */
  static const int Zero = 0;
  /*! \brief Constanta statica care identifica tipul de matrice unitara (utila
   * pentru a o trimite in constructor). Daca matricea nu este patratica, face
   * fallback pe None.
   */
  static const int Unit = 1;
  /*! \brief Constanta statica care identifica tipul de matrice neinitializata
   * (valoarea implicita din constructor).
   */
  static const int None = 2;

  /*! \brief Functie care creaza o matrice de <b>n_lines</b> linii si
   * <b>n_columns</b> coloane, copiind valorile din matricea initiala incepand
   * de la <b>(start_lin,start_col)</b>
   * \param start_lin Linia de la care sa inceapa copierea din matrice
   * \param start_col Coloana de la care sa inceapa copierea din matrice
   * \param n_lines Numarul de linii al matricei create
   * \param n_columns Numarul de coloane al matricei create
   * \return Matricea rezultata prin copierea portiunii specificate din
   * matricea initiala. Daca dimensiunile specificate ies din matricea
   * initiala, se intoarce o matrice cu 0 linii si 0 coloane.
   */ 
  Matrix<Type> submatrix(int start_lin,
                         int start_col,
                         int n_lines,
                         int n_columns) const {
    if (start_lin + n_lines > n_lin ||
        start_col + n_columns > n_col) {
      std::cerr << "Error! Request for sumatrix of size (" << n_lines
          << "," << n_columns << ") exceeds size of original matrix!\n";
      return Matrix<Type>();
    } else {
      Matrix<Type> result(n_lines, n_columns);
      for (int i = 0; i < n_lines; i++){
        for (int j = 0; j < n_columns; j++){
          result[i][j] = m[(start_lin+i)*n_col+start_col+j];
        }
      }
      return result;
    }
  }

  /*! \brief Functie care copie continutul unei matrice in matricea curenta,
   * incepand de la elementul de coordonate <b>(start_lin,start_col)</b>.
   * \param start_lin Linia de la care sa inceapa copierea in matrice
   * \param start_col Coloana de la care sa inceapa copierea in matrice
   * \param source Matricea al carei continut se copie.
   * \return O referinta la matricea in care se copie. Daca copierea ar
   * depasi limitele matricei destinatie, atunci ea nu se efectueaza.
   */   
  Matrix<Type>& copy(int start_lin, int start_col, const Matrix<Type>& source) {
    if (start_lin + source.n_lin > n_lin ||
        start_col + source.n_col > n_col){
      std::cerr << "Error! Copy-ing matrix of size (" << source.n_lin << ","
          << source.n_col << ") will exceed size of original matrix!\n";
    } else {
      for (int i = 0; i < source.n_lin; i++){
        for (int j = 0; j < source.n_col; j++){
          m[(i+start_lin)*n_col+j+start_col] = source[i][j];
        }
      }
    }
    return (*this);
  }

  /*! \brief Operator care da acces la adresa de inceput a unei linii, astfel 
   * incat elementele din matrice sa poata fi referite prin seturi de
   * paranteze patrate ca si intr-un array bidimensinoal.
   */
  inline Type* operator[](int line) const {
    return m+line*n_col; 
  }

  /*! \brief Operatorul de atribuire intre doua obiecte de tip matrice.
   * Are ca efect clonarea matricei din dreapta.
   * \param right Matricea care se atribuie
   * \return O referinta la obiectul din stanga operatorului egal,
   * utila pentru chaining.
   */
  Matrix<Type>& operator= (const Matrix<Type>& right) {
    if (this != &right) {
      if (n_lin > 0 && n_col > 0) {
        deallocate();
      }
      n_lin = right.n_lin;
      n_col = right.n_col;
      allocate(right);
    }
    return (*this);
  }

  /*! \brief Operatorul de adunare la continutul unei matrici.
   * <b>Atentie!</b> Adunarea incrementala este mai rapida decat atribuirea
   * unei sume de matrici.  
   * \param right Matricea care se aduna
   * \return O referinta la matricea la care se aduna (pentru chaining).
   * Daca adunarea nu este legala, atunci se ignora operatia.
   */
  Matrix<Type>& operator+= (const Matrix<Type>& right) {
    if (this == &right) {
      std::cerr << "Error! Ignoring increment with self!"
          << " Try m=m+m instead of m+=m!\n";
    } else if (n_lin != right.n_lin || n_col != right.n_col) {
      std::cerr << "Warning! Ingnoring \"+=\" between matrices of sizes ("
          << n_lin << "," << n_col << ") and (" << right.n_lin << ","
          << right.n_col << ").\n";
    } else {
      for (int i = 0; i < n_lin; i++){
        for (int j = 0; j < n_col; j++){
          m[i*n_col+j] += right[i][j];
        }
      }
    }
    return (*this);
  }

  /*! \brief Operatorul de scadere din continutul unei matrici.
   * <b>Atentie!</b> Scaderea incrementala este mai rapida decat atribuirea
   * unei diferente de matrici.  
   * \param right Matricea care se scade
   * \return O referinta la matricea din care se scade (pentru chaining).
   * Daca scaderea nu este legala, atunci se ignora operatia.
   */
  Matrix<Type>& operator-= (const Matrix<Type>& right) {
    if (this == &right){
      std::cerr << "Error! Ignoring decrement by self!"
          << " Try m=m-m instead of m-=m!\n";
    } else if (n_lin != right.n_lin || n_col != right.n_col){
      std::cerr << "Warning! Ingnoring \"-=\" between matrices of sizes ("
          << n_lin << "," << n_col << ") and (" << right.n_lin << ","
          << right.n_col << ").\n";  
    } else {
      for (int i = 0; i < n_lin; i++){
        for (int j = 0; j < n_col; j++){
          m[i*n_col+j] -= right[i][j];
        }
      }
    }
    return (*this);
  }

  friend std::istream& operator>> <>(std::istream&, Matrix<Type>& right);
  friend std::ostream& operator<< <>(std::ostream&, const Matrix<Type> right);
  friend Matrix<Type> operator* <>(const Matrix<Type>& left, const Matrix<Type>& right);
  friend Matrix<Type> operator+ <>(const Matrix<Type>& left, const Matrix<Type>& right);
  friend Matrix<Type> operator- <>(const Matrix<Type>& left, const Matrix<Type>& right);
};

#endif

