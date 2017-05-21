import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.TimeUnit;

/**
 * Clasa MapWorker implementeaza un worker ce se ocupa de paradigma REDUCE
 * Un Worker se va ocupa de concatenarea a 2 HashMap-uri intrunul singur
 * Rezultatl va fi adaugat in coada pentru a putea fi utilizat de alt HashMap
 * Am impementat concatenarea utilizand o coada blocanta in cazul in care
 * in coada nu se afla cel putin 2 HashMap-uri necesare rezolvarii functiei de REDUCE
 */
public class ReduceWorker implements Runnable {
	
	// Index document
	private int doc_number;

	// Constructor
	public ReduceWorker(int doc_number) {
		this.doc_number = doc_number;
	}

	
	public void reduceHash() throws InterruptedException {

		HashMap<String, WordCounter> WordDict1 = null;
		HashMap<String, WordCounter> WordDict2 = null;
		
		// Se asteapta pana ce se poate extrage un Hash din coada 
		while(WordDict1 == null) {
//			boolean x = false;
			WordDict1 = Main.indexedDoc.get(doc_number).poll(10, TimeUnit.MILLISECONDS);
//			if (x == true)
//				System.out.println("waiting for something 1");
//			x = true;
		}
		
		// Se asteapta pana ce se poate extrage cel de-al 2-lea Hash 
		while(WordDict2 == null)
			WordDict2 = Main.indexedDoc.get(doc_number).poll(10, TimeUnit.MILLISECONDS);
		

		// Se concateneaza cele 2 HashMap-uri	
		for(Map.Entry<String, WordCounter> entry : WordDict1.entrySet()) {
			
			String word = entry.getKey(); 
			int count = entry.getValue().count;
			
			if (WordDict2.containsKey(word))
				WordDict2.get(word).increment(count);
			else
				WordDict2.put(word, entry.getValue());

		}
		
		// Se adauga in coada HashMap-ul rezultant
		Main.indexedDoc.get(doc_number).add(WordDict2);
		
		
	}

	@Override
	public void run() {
		try {

			reduceHash();

		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
