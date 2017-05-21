#include <iostream>
#include <stdint.h>

/* Vom imparti cele doua numere in formatul:
 * n1 = x1 * (2^exp) + x2
 * n2 = y1 * (2^exp) + y2
 *
 * Deci produsul final devine:
 *
 * (n1 * n2) = alpha * (2^(2*exp)) + beta * (2^exp) + gamma
 *
 * unde:
 *  alpha = x1 * y1
 *  beta = (x1 + x2) * (y1 + y2) - alpha - gamma
 *  gamma = x2 * y2
 */
uint64_t karatsuba(uint64_t x, uint64_t y, unsigned char exp)
{
  /* Daca exponentul este 0, atunci inmulteste direct. */
  if (exp < 2) {
    return x * y;
  }

  /* Cream o masca pentru a extrage bitii din a si din b. */
  uint64_t mask = (((uint64_t) 1) << exp) - 1;

  /* Calculam x1, x2, y1, y2. */
  uint64_t x1 = x >> exp;
  uint64_t x2 = x & mask;
  uint64_t y1 = y >> exp;
  uint64_t y2 = y & mask;

  /* Calculam alpha, beta si gamma. */
  uint64_t alpha = karatsuba(x1, y1, exp / 2);
  uint64_t gamma = karatsuba(x2, y2, exp / 2);
  uint64_t beta = karatsuba((x1 + x2), (y1 + y2), exp / 2) - alpha - gamma;

  /* Intoarcem rezultatul final. */
  return alpha * (((uint64_t) 1) << (2 * exp)) +
      beta * (((uint64_t) 1) << exp) +
      gamma;
}

int main()
{
  /* Citim doua numere fara semn, de 32 biti, pe care le vom inmulti. */
  uint32_t n1, n2;
  std::cin >> n1 >> n2;

  /* Afisam rezultatul inmultirii Karatasuba. */
  uint64_t prod_classic = ((uint64_t) n1) * ((uint64_t) n2);
  uint64_t prod_karatsuba = karatsuba(n1, n2, 16);

  if (prod_classic == prod_karatsuba) {
    std::cout << "OK!" << std::endl << "Produsul celor doua numere este: "
        << prod_classic << std::endl;
  } else {
    std::cout << "FAIL!" << std::endl << "Produsul celor doua numere este: "
        << prod_classic << std::endl << "iar Karatsuba a calculat: "
        << prod_karatsuba << std::endl;
  }

  return 0;
}

