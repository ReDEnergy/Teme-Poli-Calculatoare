import java.io.*;
import java.math.RoundingMode;
import java.text.DecimalFormat;
import java.util.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

public class Main {
	
	// Numarul de Threaduri
	public static int NR_THREADS;
	
	// NUmarul de cuvinte cautate
	public static int nr_words;
	
	// Chunck size fisier
	public static int doc_split_size;
	
	// Numarul de cuvinte de indexact pentru fiecare fisier 
	public static int nr_indexed_words;
	
	// Numarul de rezultate afisate 
	public static int nr_result;
	
	// Numarul fisierelor de indexat
	public static int nr_indexed_docs;
	
	// Array-ul de cuvinte ce urmeaza a fi cautate in fisiere
	public static ArrayList<String> words = new ArrayList<String>();
	
	// Array-ul de fisiere ce urmeaza a fi indexate
	public static ArrayList<String> docs  = new ArrayList<String>();
	
	// Numele fisierului de iesire
	public static String out_file;
	
	// Array-ul de Cozi de HashMaps pentru fiecare fisier in parte
	// Coada blocanta accesata de toate Threadurile
	public static ArrayList<LinkedBlockingQueue<HashMap<String, WordCounter>>> 
		indexedDoc = new ArrayList<LinkedBlockingQueue<HashMap<String, WordCounter>>>();

	// numarul total de cuvinte din fiecare fisier
	public static int[] word_index_count;
	
	// fisierele rezultat ce trebuie afisate
	// true		=> se afiseaza fisierul
	// false	=> nu se afiseaza fisierul 
	public static boolean[] result;
	
	

	////////////////////////////////////////////////////////////////////////////////////
	// Methods
	
	
	// Se citesc informatiile de intrare
	public static void readInput(String input_file) throws IOException {

		Scanner sc = new Scanner(new File(input_file));

		nr_words = sc.nextInt();	

		for (int i=0; i<nr_words; i++)
			words.add(sc.next());
		
		doc_split_size = sc.nextInt();
		nr_indexed_words = sc.nextInt();
		nr_result = sc.nextInt();
		nr_indexed_docs = sc.nextInt();

		word_index_count = new int[nr_indexed_docs];
		result = new boolean[nr_indexed_docs];
		
		for (int i=0; i<nr_indexed_docs; i++) {
			docs.add(sc.next());
			indexedDoc.add(new LinkedBlockingQueue<HashMap<String, WordCounter>>());
			result[i] = true;
		}

		
	//	System.out.println(nr_words);
	//	System.out.println(words.toString());
	//	System.out.println(doc_split_size);
	//	System.out.println(nr_indexed_words);
	//	System.out.println(nr_result);
	//	System.out.println(nr_indexed_docs);
	//	System.out.println(docs.toString());

		sc.close();
	}
	
	/**
	 * Functia Master pentru asignarea workerilor
	 * Fiecare worker primeste documentul, pozitia de start si lungimea chunck-ului
	 * Pentru threading am utilizat clasa ExecutorService
 	 */
	
	public static void map() throws IOException {
		
		ExecutorService taskExecutor = Executors.newFixedThreadPool(NR_THREADS);

		for (int i=0; i<nr_indexed_docs; i++) {
			
			MapWorker worker;			

			String doc_name = docs.get(i);
			
			RandomAccessFile doc = new RandomAccessFile(doc_name, "r");

			int start_point = 0;	
			int left_to_read = (int) doc.length();
			
			while (left_to_read != 0) {
				if (left_to_read > doc_split_size) {
					worker = new MapWorker(doc_name, i, start_point, doc_split_size);
					left_to_read -= doc_split_size;
				}
				else {
					worker = new MapWorker(doc_name, i, start_point, left_to_read);
					left_to_read = 0;
				}

				start_point += doc_split_size;

				taskExecutor.execute(worker);
			}
			
			doc.close();
		}
		
		waitThreadsToFinish(taskExecutor);
	}

	
	/**
	 * Functia Reduce pentru asignarea workerilor
	 * Fiecare worker primeste numarul documentului
	 * Workerii vor unii Hash-Map-urile generate in urma functiei de Map intr-un singur HashMap
	 * Fiecare Worker :
	 * 		extrage cate 2 hashmap-uri din coada fisierului respectiv
	 * 		uneste cele 2 hashmap-uri intrunul singur
	 * 		adauga hashmap-ul in coada	
 	 */
	
	public static void reduce() {
		
		ExecutorService taskExecutor = Executors.newFixedThreadPool(NR_THREADS);

		for (int i=0; i<nr_indexed_docs; i++) {
			
			ReduceWorker worker;

			int size = indexedDoc.get(i).size();
	
			while(size > 1) {
				if(indexedDoc.size() > 1) {
					worker = new ReduceWorker(i);
					taskExecutor.execute(worker);
					size--;
				}
			}
		}

		waitThreadsToFinish(taskExecutor);
	}

	/**
	 * Functia de reduce asigneaza fiecarui Thread un worker ce ordoneaza
	 * si calculeaza valoarea ultimul cuvant indexat 
	 */
	public static void reduceSolution() {
		
		ExecutorService taskExecutor = Executors.newFixedThreadPool(NR_THREADS);
		
		for (int i=0; i<nr_indexed_docs; i++) {
			ReduceSolution worker = new ReduceSolution(i);
			taskExecutor.execute(worker);
		}		
		
		waitThreadsToFinish(taskExecutor);
	}	

	/**
	 * Functie ce opreste executarea unui ExecutorService
	 * Se asteapta finalizarea tuturor workerilor
	 * @param taskExecutor
	 */
	public static void waitThreadsToFinish(ExecutorService taskExecutor) {
		taskExecutor.shutdown();
		try {
			while(taskExecutor.awaitTermination(1, TimeUnit.SECONDS) == false) {
				System.out.println("waiting threads to finish ...");
			}
		} catch (InterruptedException e) {
			e.printStackTrace();
		}		
	}	
	
	/**
	 * Functie de debuging pentru printarea unui HashMap
	 */
	public static void printMap() {

		System.out.println("--------------------------------------------------------------------\n");

		for (int i=0; i<nr_indexed_docs; i++) {
			System.out.println("Doc " + i + " => HashMaps = " + indexedDoc.get(i).size());
			System.out.println(indexedDoc.get(i));
		}

		System.out.println("--------------------------------------------------------------------\n");
	}
	
	/**
	 * Functie de debuging pentru printarea rezultatului final
	 */
	public static void printDebug() {
		for (int i = 0; i < nr_indexed_docs; i++) {
			System.out.println("Doc: " + i + " index_value => " + " words => " + word_index_count[i]);
		}
	}
	
	/**
	 * Functia de afisarea a rezultatului in fisierul de output
	 */
	public static void writeAnswerToFile() throws IOException {
		
		FileWriter out = new FileWriter(out_file);
		
		HashMap<String, WordCounter> Dict;
		
		DecimalFormat df = new DecimalFormat("0.00");
		df.setRoundingMode(RoundingMode.DOWN);		

		double freq;
		
		out.write("Rezultate pentru: (");

		for (int i = 0; i < nr_words; i++) {
			out.write(words.get(i));
			if(i < (nr_words - 1))
				out.write(", ");
		}
			
		out.write(")\n");
		
		int result_counter = 0;

		for (int i = 0; i < nr_indexed_docs; i++) {
			if (result[i] == true) {

				if(result_counter++ == nr_result)
					break;

				out.write("\n");	
				
				Dict = indexedDoc.get(i).peek();

				out.write(docs.get(i) + " (");

				for(int k=0; k < nr_words; k++) {
					freq = (float)Dict.get(words.get(k)).count / word_index_count[i] * 100;
					out.write(df.format(freq));
					if(k < (nr_words - 1))
						out.write(", ");
				}
				out.write(")");
			}
		}
		
		out.close();
		
	}
	
	/**
	 * Functia Main
	 * @param args
	 * @throws IOException
	 */
	public static void main(String args[]) throws IOException {
		
		// Timing for Windows
		long startTime = System.currentTimeMillis();
		
		NR_THREADS = Integer.parseInt(args[0]);
		
		out_file = args[2];
		
		// Se citeste inputul
		readInput(args[1]);

		// Map
		map();
		
		// Reduce Reduce Reduce
		
		reduce();

		reduceSolution();
		
		// Solution
		
		writeAnswerToFile();

		System.out.println("Query Finished");
		
		// Print Timing
		long endTime = System.currentTimeMillis();
		System.out.println("Took "+(endTime - startTime) + "ms"); 		
		
	}
}

