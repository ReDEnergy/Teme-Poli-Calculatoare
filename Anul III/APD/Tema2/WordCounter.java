/**
 * Clasa WordCounter este necesara pentru a mentine numarul de aparitii al unui cuvant intr-un fisier
 * Utilizand functia de increment() se poate actualiza usor numarul de aparitii al unui cuvant
 */
public class WordCounter {
	public int count = 0;

	public WordCounter(int count) {
		this.count = count;
	}
	
	public void increment() {
		count++;
	}
	
	public void increment(int add) {
		count += add;
	}
	
	public String toString() {
		return ""+count;
	}
}
