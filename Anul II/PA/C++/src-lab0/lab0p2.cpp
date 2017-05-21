#include <iostream>
#include <string>

int main() 
{
  /* Sirul din care stergem si sirul pe care il stergem. */
  std::string text, pattern;

  /* Citim cele doua siruri. */
  std::cin >> text >> pattern;

  /* Stergem intr-o bucla. */
  for (size_t position = text.find(pattern);
       position != std::string::npos;
       position = text.find(pattern)) {
    text.erase(position, pattern.length());
    std::cerr << "Next deletion: " << text << std::endl;
  }

  return 0;
}

