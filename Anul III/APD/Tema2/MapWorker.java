import java.io.IOException;
import java.io.RandomAccessFile;
import java.util.HashMap;
import java.util.Scanner;

/**
 * Clasa MapWorker implementeaza un worker ce se ocupa de paradigma MAP 
 * Fiecare worker citeste din fisier un chuck-size primit
 * Se obtin toate cuvintele formate din litere
 * Se construieste un HashMap (cuvant, numar de aparitii) ce este adaugat in Codata Shared din Main
 */
public class MapWorker implements Runnable {
	
	// Indicele fisierului citit
	private int doc_number;

	// Variabila de fisier
	private RandomAccessFile doc;

	// Pozitia de start de la care se va citi
	private int start_pos;
	
	// Numarul de elemente citite
	private int count;
	
	/**
	 * Constructor
	 * @param doc
	 * @param doc_number
	 * @param start_pos
	 * @param count
	 * @throws IOException
	 */
	public MapWorker(String doc, int doc_number, int start_pos, int count) throws IOException {
		this.doc = new RandomAccessFile(doc, "r");		
		this.doc_number = doc_number;
		this.start_pos = start_pos;
		this.count = count;
		
	}

	/**
	 * Functia RUN ce fa fi rulata de fiecare THREAD
	 */
	@Override	
	public void run() {
		try {

			boolean previous_letter = false;
			
			// Se ferifica daca caracterul actual este litera si daca caracterul precedent a fost litera
			// caz in care se va sari peste acest cuvant deoarece a fost indexat de catre un alt worker
			
			if (start_pos > 0) {
				doc.seek(start_pos-1);
				previous_letter = Character.isLetter(doc.readByte());
			}
			
				doc.seek(start_pos + count - 1);
			
			if (Character.isLetter(doc.readByte()) == true) {
				while (Character.isLetter(doc.readByte()) == true)
					count++;
			}

			// Se citeste din fisier numarul de caractere necesare
			byte[] buffer = new byte[count];
			
			doc.seek(start_pos);
			doc.read(buffer);

			Scanner sc = new Scanner(new String(buffer));
			sc.useDelimiter("[^a-zA-Z]+");	

			if (Character.isLetter(buffer[0]) == true && previous_letter == true) {
				sc.next();
			}
			
			// Se genereaza un HashMap cu numarul de aparitii pentru fiecare cuvant valid 
			// Clasa WordCounter este folosita pentru a putea incrementa usor numarul de aparitii al unui cuvant 
			HashMap<String, WordCounter> Dict = new HashMap<String, WordCounter>();
			
			while(sc.hasNext()) {
				
				String word = sc.next().toLowerCase();
				
				if (Dict.containsKey(word))
					Dict.get(word).increment();
				else
					Dict.put(word, new WordCounter(1));

			}

			sc.close();
			doc.close();
			
			Main.indexedDoc.get(doc_number).add(Dict);
			
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}
