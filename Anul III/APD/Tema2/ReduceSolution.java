import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;


/**
 * Comparator necesar sortarii unui HashMap dupa valori  
 */
class HashMapComp implements Comparator<Map.Entry<String, WordCounter>> {
	
	public int compare(Map.Entry<String, WordCounter> A, Map.Entry<String, WordCounter> B) {
		return B.getValue().count - A.getValue().count;
	}
}

/**
 * Clasa ReduceKeep implementeaza sortarea si salvarea valorii ultimului cuvant indexat
 * (cuvantul cu numarul N = nr_indexed_words)  
 * De asemenea se calculeaza si salveaza numarul total de cuvinte necesar pentru a calcula FRECVENTA 
 */
public class ReduceSolution implements Runnable {

	// Indice Document
	private int doc_number;

	// Constructor
	public ReduceSolution(int doc_number) {
		this.doc_number = doc_number;
	}
	

	@Override
	public void run() {
		
		// Se Sorteaza HashMap-ul dupa ordinea descrescatoare a valorilor
		HashMap<String, WordCounter> Dict = Main.indexedDoc.get(doc_number).peek();
		
		ArrayList<Map.Entry<String, WordCounter>> sortedDict = new ArrayList<Map.Entry<String, WordCounter>>();

		for(Map.Entry<String, WordCounter> entry : Dict.entrySet())
			sortedDict.add(entry);
		
		Collections.sort(sortedDict, new HashMapComp());
		
		// Se pastreaza valoarea ultimului cuvant necesar pentru indexare
		int last_word_indexed_value  =  sortedDict.get(Main.nr_indexed_words - 1).getValue().count;
		
		// Se calculeaza numarul total de cuvinte din fisier necesar pentru a calcula FRECVENTA
		int count = 0;
		for (int i = 0; i < sortedDict.size(); i++) {
			Map.Entry<String, WordCounter> entry = sortedDict.get(i);
			count += entry.getValue().count;
		}
		
		Main.word_index_count[doc_number] = count;
		
//		System.out.println("Doc" + doc_number + sortedDict);
//		System.out.println("Doc" + doc_number + " => " + count);
		
		
		// Fiecare cuvant este cautat in HashMap-ul fisierului respectiv

		ArrayList<String> words = Main.words;		
		
		for (String word : words) {
			// Daca un cuvant nu este gasit se opreste executia iar fisierul este scos din rezultat
			if (Dict.containsKey(word) == false) {
				Main.result[doc_number] = false;
				break;
			}
			
			// Daca se gaseste cuvantul in HashMap, se verifica ca voloarea acestuia 
			// este mai mare sau egala cu valoarea ultimului cuvant indexat
			// In caz contrar se scoate fisierul din rezultat
			else {
				if(Dict.get(word).count < last_word_indexed_value) {
					Main.result[doc_number] = false;
					break;
				}
			}
		}		
		
	}
}
