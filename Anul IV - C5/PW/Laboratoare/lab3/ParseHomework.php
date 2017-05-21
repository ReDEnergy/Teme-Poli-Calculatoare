<?php 

	function generateRandomString($length = 16) {
	    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
	    $randomString = '';
	    for ($i = 0; $i < $length; $i++) {
	        $randomString .= $characters[rand(0, strlen($characters) - 1)];
	    }
	    return $randomString;
	}
	
	interface IParseHomework {
		public function setNumberOfTasks();
		public function initScore();		
		public function checkTasks();
		public function sortTasks();
	};

	class ParseHomework implements IParseHomework
	{
		public $nume;
		public $prenume;
		public $grupa;
		public $tema;
		private $numarTaskuri;
		private $scores;
		
		function __construct($tema) {
			$values = explode('.', $tema);
			$this->nume = $values[0];
			$this->prenume = $values[1];
			$this->grupa = $values[2];
			$this->tema = $values[3];
			$this->scores = array();	
		}
		
		function __toString() {
			return "[$this->nume,$this->prenume,$this->grupa,$this->tema]";
		}
		
		public function setNumberOfTasks() {
			$numbers;
			preg_match_all('!\d+!', $this->tema, $numbers);
			$this->numarTaskuri = intval(rand(2, 10) / $numbers[0][0]) + 1;
			echo $this->numarTaskuri;
		}
		
		public function initScore() {
			for ($i=0; $i<$this->numarTaskuri; $i++)
				array_push($this->scores, array());
		}
		
		public function checkTasks() {
			for ($i=0; $i<$this->numarTaskuri; $i++) {
				$this->scores[$i][0] = rand(0, 100);
				$this->scores[$i][1] = generateRandomString();
			}
			var_dump($this->scores);
		}
		
		public function sortTasks() {
			$tasks = $this->numarTaskuri;
			for ($i=0; $i<$tasks; $i++) {
				for ($j=0; $j<$tasks; $j++) {
					if ($this->scores[$i][0] < $this->scores[$j][0]) {
						$c = $this->scores[$i];
						$this->scores[$i] = $this->scores[$j];
						$this->scores[$j] = $c;
					}
				}
			}
			var_dump($this->scores);
		}
	};
?>